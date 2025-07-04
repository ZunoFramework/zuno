#include "zuno/http_server.hpp"
#include "zuno/app.hpp"
#include "zuno/logger.hpp"
#include "zuno/request.hpp"
#include "zuno/response.hpp"
#include "zuno/tcp_stream_adapter.hpp"
#include "zuno/tls_stream_adapter.hpp"

namespace zuno
{

HttpServer::HttpServer(asio::io_context& ctx, int port, const App& app, std::shared_ptr<asio::ssl::context> sslCtx)
    : io_(ctx), acceptor_(ctx, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)), socket_(ctx), sslContext_(sslCtx), app_(app)
{
}

void HttpServer::start()
{
    doAccept();
}

void HttpServer::doAccept()
{
    if (app_.tlsEnabled_ && sslContext_)
    {
        auto tlsSocket = std::make_shared<asio::ssl::stream<asio::ip::tcp::socket>>(io_, *sslContext_);
        acceptor_.async_accept(tlsSocket->lowest_layer(),
                               [this, tlsSocket](std::error_code ec)
                               {
                                   if (!ec)
                                   {
                                       tlsSocket->async_handshake(asio::ssl::stream_base::server,
                                                                  [this, tlsSocket](std::error_code ec)
                                                                  {
                                                                      if (!ec)
                                                                      {
                                                                          auto stream = std::make_shared<TlsStreamAdapter>(tlsSocket);
                                                                          handleConnection(stream);
                                                                      }
                                                                  });
                                   }
                                   doAccept();
                               });
    }
    else
    {
        acceptor_.async_accept(socket_,
                               [this](std::error_code ec)
                               {
                                   if (!ec)
                                   {
                                       auto stream = std::make_shared<TcpStreamAdapter>(std::move(socket_));
                                       handleConnection(stream);
                                   }
                                   doAccept();
                               });
    }
}

void HttpServer::handleConnection(StreamAdapterPtr stream)
{
    try
    {
        auto start = std::chrono::steady_clock::now();

        asio::streambuf buffer;
        stream->read_until(buffer, "\r\n\r\n");

        std::istream streamIn(&buffer);
        std::string requestLine;
        std::getline(streamIn, requestLine);
        if (!requestLine.empty() && requestLine.back() == '\r') requestLine.pop_back();

        std::string method, path, version;
        std::istringstream rl(requestLine);
        rl >> method >> path >> version;

        std::unordered_map<std::string, std::string> headers;
        std::string line;
        while (std::getline(streamIn, line) && line != "\r")
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
                stream->read(buffer, remaining);
            }

            std::istream bodyStream(&buffer);
            body.resize(contentLength);
            bodyStream.read(&body[0], contentLength);
        }

        std::unordered_map<std::string, std::string> params;
        auto handler = app_.resolveHandler(method, path, params);

        zuno::Request req(path, stream);
        req.setMethod(method);
        req.setBody(std::move(body));
        req.params = std::move(params);
        req.headers = std::move(headers);

        zuno::Response res(stream);

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
        if (reason.find("end of file") != std::string::npos) return;

        log::error("{}", reason);
    }
}

} // namespace zuno