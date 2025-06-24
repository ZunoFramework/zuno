#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include "request.hpp"
#include "response.hpp"
#include "route.hpp"

namespace zuno {

using Handler = std::function<void(const Request&, Response&)>;

class App {
public:
    void get(const std::string& path, Handler handler);
    void post(const std::string& path, Handler handler);
    void put(const std::string& path, Handler handler);
    void del(const std::string& path, Handler handler); // delete es palabra reservada

    void listen(int port);

    Handler resolveHandler(const std::string& method, const std::string& path,std::unordered_map<std::string, std::string>& outParams) const;

private:
    std::unordered_map<std::string, std::vector<Route>> routes;

    void addRoute(const std::string& method, const std::string& path, Handler handler);
};

} // namespace zuno
