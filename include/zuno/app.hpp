#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include "request.hpp"
#include "response.hpp"
#include "route.hpp"

namespace zuno {

using Handler = std::function<void(const Request&, Response&)>;
using Next = std::function<void()>;
using Middleware = std::function<void(Request&, Response&, Next)>;

class HttpServer;

class App {
    friend class HttpServer;
public:
    void get(const std::string& path, Handler handler);
    void post(const std::string& path, Handler handler);
    void put(const std::string& path, Handler handler);
    void del(const std::string& path, Handler handler);

    void use(Middleware m) {
        middlewares_.push_back(std::move(m));
    }

    void listen(int port);

    Handler resolveHandler(const std::string& method, const std::string& path,std::unordered_map<std::string, std::string>& outParams) const;

private:
    std::unordered_map<std::string, std::vector<Route>> routes;
    std::vector<Middleware> middlewares_;

    void addRoute(const std::string& method, const std::string& path, Handler handler);
};

} // namespace zuno
