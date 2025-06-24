#pragma once
#include <regex>
#include <string>
#include <vector>
#include <functional>
#include "request.hpp"
#include "response.hpp"

namespace zuno {

struct Route {
    std::regex pattern;
    std::vector<std::string> paramNames;
    std::function<void(const Request&, Response&)> handler;
    std::string rawPath;
};

Route compileRoute(const std::string& path,
                   std::function<void(const Request&, Response&)> handler);

}
