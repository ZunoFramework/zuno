#pragma once

#include <asio.hpp>
#include <asio/ssl.hpp>
#include <functional>
#include <memory>
#include <unordered_map>
#include "app.hpp"
#include "common.hpp"
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

    HttpServer(asio::io_context& ctx, int port, const App& app, std::shared_ptr<asio::ssl::context> sslCtx = nullptr);

    void start();

   private:
    asio::ip::tcp::acceptor acceptor_;
    asio::ip::tcp::socket socket_;
    asio::io_context& io_;
    std::shared_ptr<asio::ssl::context> sslContext_;
    std::unordered_map<std::string, Handler> routes_;

    void doAccept();
    void handleConnection(StreamAdapterPtr stream);
};

} // namespace zuno
