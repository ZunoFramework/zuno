#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include "common.hpp"
#include "route.hpp"

namespace zuno
{

class HttpServer;

class App
{
    friend class HttpServer;

   public:
    void get(const std::string& path, Handler handler);
    void get(const std::string& path, std::initializer_list<Middleware> mws, Handler handler);
    void post(const std::string& path, Handler handler);
    void post(const std::string& path, std::initializer_list<Middleware> mws, Handler handler);
    void put(const std::string& path, Handler handler);
    void put(const std::string& path, std::initializer_list<Middleware> mws, Handler handler);
    void del(const std::string& path, Handler handler);
    void del(const std::string& path, std::initializer_list<Middleware> mws, Handler handler);

    void use(Middleware m)
    {
        middlewares_.push_back(std::move(m));
    }

    void listen(int port);

    std::function<void(Request&, Response&)> resolveHandler(
        const std::string& method, const std::string& path,
        std::unordered_map<std::string, std::string>& outParams) const;

   private:
    std::unordered_map<std::string, std::vector<Route>> routes;
    std::vector<Middleware> middlewares_;

    void addRoute(const std::string& method, const std::string& path, Handler handler);
    void addRoute(const std::string& method, const std::string& path,
                  std::initializer_list<Middleware> mws, Handler handler);
};

} // namespace zuno
