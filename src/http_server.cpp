#include "zuno/http_server.hpp"
#include "zuno/logger.hpp"
#include "zuno/request.hpp"
#include "zuno/response.hpp"
#include "zuno/app.hpp"

namespace zuno {

HttpServer::HttpServer(asio::io_context& ctx, int port, const App& app)
    : acceptor_(ctx, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)),
      socket_(ctx),
      app_(app) {}

void HttpServer::start() {
    doAccept();
}

void HttpServer::doAccept() {
    acceptor_.async_accept(socket_, [this](std::error_code ec) {
        if (!ec) {
            handleConnection(std::move(socket_));
        }
        doAccept();
    });
}

void HttpServer::handleConnection(asio::ip::tcp::socket socket) {
    try {
        asio::streambuf buffer;
        asio::read_until(socket, buffer, "\r\n\r\n");

        std::istream stream(&buffer);
        std::string requestLine;
        std::getline(stream, requestLine);
        if (requestLine.back() == '\r') requestLine.pop_back();

        std::string method, path, version;
        std::istringstream lineStream(requestLine);
        lineStream >> method >> path >> version;

        std::unordered_map<std::string, std::string> headers;
        std::string header;
        while (std::getline(stream, header) && header != "\r") {
            if (header.back() == '\r') header.pop_back();
            auto pos = header.find(":");
            if (pos != std::string::npos) {
                std::string key = header.substr(0, pos);
                std::string value = header.substr(pos + 1);
                while (!value.empty() && value[0] == ' ') value.erase(0, 1);
                headers[key] = value;
            }
        }

        std::size_t contentLength = 0;
        if (headers.count("Content-Length")) {
            contentLength = std::stoul(headers["Content-Length"]);
        }

        std::string body;
        if (contentLength > 0) {
            std::size_t already = buffer.size();
            std::size_t remaining = (contentLength > already) ? (contentLength - already) : 0;

            if (remaining > 0) {
                asio::read(socket, buffer, asio::transfer_exactly(remaining));
            }

            std::istream bodyStream(&buffer);
            body.resize(contentLength);
            bodyStream.read(&body[0], contentLength);
        }

        std::unordered_map<std::string, std::string> params;
        auto handler = app_.resolveHandler(method, path, params);

        Request req(path, socket);  
        req.params = std::move(params);
        req.headers = std::move(headers);
        req.body = std::move(body);  

        Response res(socket);

        if (handler) {
            handler(req, res);
        } else {
            res.send("Not found", 404);
        }

    } catch (const std::exception& ex) {
        std::cerr << "[ZUNO] Error handling connection: " << ex.what() << "\n";
    }
}

}