#pragma once

#include <asio.hpp>
#include <memory>
#include <unordered_map>
#include <functional>
#include "request.hpp"
#include "response.hpp"
#include "zuno/logger.hpp"
#include "app.hpp"


namespace zuno {

class HttpServer {
public:
    using Handler = std::function<void(const Request&, Response&)>;
    const App& app_;

    HttpServer(asio::io_context& ctx, int port, const App& app);

    void start();

private:
    asio::ip::tcp::acceptor acceptor_;
    asio::ip::tcp::socket socket_;
    std::unordered_map<std::string, Handler> routes_;

    void doAccept();
    void handleConnection();
};

}
