#include "zuno/http_server.hpp"
#include "zuno/app.hpp"
#include "zuno/logger.hpp"
#include "zuno/request.hpp"
#include "zuno/response.hpp"

using namespace zuno::log::color;

namespace zuno
{

HttpServer::HttpServer(asio::io_context& ctx, int port, const App& app)
    : acceptor_(ctx, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)), socket_(ctx), app_(app)
{
}

void HttpServer::start()
{
    doAccept();
}

void HttpServer::doAccept()
{
    acceptor_.async_accept(socket_,
                           [this](std::error_code ec)
                           {
                               if (!ec)
                               {
                                   handleConnection(std::move(socket_));
                               }
                               doAccept();
                           });
}

void HttpServer::handleConnection(asio::ip::tcp::socket socket)
{
    try
    {
        auto start = std::chrono::steady_clock::now();

        asio::streambuf buffer;
        asio::read_until(socket, buffer, "\r\n\r\n");

        std::istream stream(&buffer);
        std::string requestLine;
        std::getline(stream, requestLine);
        if (!requestLine.empty() && requestLine.back() == '\r') requestLine.pop_back();

        std::string method, path, version;
        std::istringstream rl(requestLine);
        rl >> method >> path >> version;

        std::unordered_map<std::string, std::string> headers;
        std::string line;
        while (std::getline(stream, line) && line != "\r")
        {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            auto pos = line.find(":");
            if (pos != std::string::npos)
            {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                while (!value.empty() && value.front() == ' ') value.erase(0, 1);
                headers[key] = value;
            }
        }

        std::size_t contentLength = 0;
        if (headers.count("Content-Length"))
        {
            contentLength = std::stoul(headers["Content-Length"]);
        }

        std::string body;
        if (contentLength > 0)
        {
            std::size_t already = buffer.size();
            std::size_t remaining = (contentLength > already) ? (contentLength - already) : 0;
            if (remaining > 0)
            {
                asio::read(socket, buffer, asio::transfer_exactly(remaining));
            }

            std::istream bodyStream(&buffer);
            body.resize(contentLength);
            bodyStream.read(&body[0], contentLength);
        }

        std::unordered_map<std::string, std::string> params;
        auto handler = app_.resolveHandler(method, path, params);

        zuno::Request req(path, socket);
        req.method = method;
        req.params = std::move(params);
        req.headers = std::move(headers);
        req.body = std::move(body);

        zuno::Response res(socket);

        std::size_t index = 0;
        std::function<void()> next = [&]()
        {
            if (index < app_.middlewares_.size())
            {
                auto current = app_.middlewares_[index++];
                current(req, res, next);
            }
            else if (handler)
            {
                handler(req, res);
            }
            else
            {
                res.status(404).send("Not Found");
            }
        };

        next();

        auto end = std::chrono::steady_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        log::request(method, path, res.statusCode(), ms);
    }
    catch (const std::exception& ex)
    {
        std::string reason(ex.what());
        if (reason.compare("end of file") > -1) return;

        log::error(reason);
    }
}
} // namespace zuno