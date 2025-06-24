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
            handleConnection();
        }
        doAccept();
    });
}

void HttpServer::handleConnection() {
    try {
        asio::streambuf buf;
        asio::read_until(socket_, buf, "\r\n\r\n");

        std::istream stream(&buf);
        std::string method, path, version;
        stream >> method >> path >> version;

        zuno::Request req(path);
        zuno::Response res(socket_);

        auto start = std::chrono::steady_clock::now();

        int status = 200;
        std::unordered_map<std::string, std::string> params;
        auto handler = app_.resolveHandler(method, path, params);
        req.params = params;

        if (handler) {
            handler(req, res);
        } else {
            status = 404;
            res.send("Not Found", status);
        }

        auto end = std::chrono::steady_clock::now();
        long duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        zuno::log::request(method, path, status, duration);

    } catch (const std::exception& e) {
        std::string msg = e.what();
        if (msg.find("End of file") == std::string::npos) {
            std::cerr << "[ZUNO] Request error: " << e.what() << "\n";
    }
    }

    socket_.close();
}


}
