#pragma once

#include <asio.hpp>
#include <functional>
#include <memory>
#include <unordered_map>
#include "app.hpp"
#include "request.hpp"
#include "response.hpp"
#include "zuno/logger.hpp"

namespace zuno
{

class HttpServer
{
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
    void handleConnection(asio::ip::tcp::socket socket);
};

} // namespace zuno
