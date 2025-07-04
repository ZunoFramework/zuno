#pragma once

#include <asio/ssl.hpp>
#include <functional>
#include <string>
#include <unordered_map>
#include "common.hpp"
#include "route.hpp"

namespace zuno
{

class HttpServer;

struct TLSConfig
{
    std::string certFile;
    std::string keyFile;
    std::string dhParamsFile;       // Opcional: forward secrecy
    bool requireClientCert = false; // to mTLS
    std::string caCertFile;         // to verify clients if mTLS
};

class App
{
    friend class HttpServer;

   public:
    void get(const std::string& path, Handler handler);
    void post(const std::string& path, Handler handler);
    void put(const std::string& path, Handler handler);
    void patch(const std::string& path, Handler handler);
    void del(const std::string& path, Handler handler);
    void options(const std::string& path, Handler handler);

    void get(const std::string& path, std::initializer_list<Middleware> mws, Handler handler);
    void post(const std::string& path, std::initializer_list<Middleware> mws, Handler handler);
    void put(const std::string& path, std::initializer_list<Middleware> mws, Handler handler);
    void patch(const std::string& path, std::initializer_list<Middleware> mws, Handler handler);
    void del(const std::string& path, std::initializer_list<Middleware> mws, Handler handler);
    void options(const std::string& path, std::initializer_list<Middleware> mws, Handler handler);

    void use(Middleware m)
    {
        middlewares_.push_back(std::move(m));
    }

    void useTLS(const TLSConfig& config);

    void listen(int port);

    std::function<void(Request&, Response&)> resolveHandler(const std::string& method, const std::string& path,
                                                            std::unordered_map<std::string, std::string>& outParams) const;

   private:
    std::unordered_map<std::string, std::vector<Route>> routes;
    std::vector<Middleware> middlewares_;
    bool tlsEnabled_ = false;
    std::shared_ptr<asio::ssl::context> tlsContext_;

    void addRoute(const std::string& method, const std::string& path, Handler handler);
    void addRoute(const std::string& method, const std::string& path, std::initializer_list<Middleware> mws, Handler handler);
};

} // namespace zuno
