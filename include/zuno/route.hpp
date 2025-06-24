#pragma once
#include <functional>
#include <regex>
#include <string>
#include <vector>
#include "common.hpp"

namespace zuno
{

struct Route
{
    std::regex pattern;
    std::vector<std::string> paramNames;
    std::function<void(const Request&, Response&)> handler;
    std::vector<Middleware> middlewares;
    std::string rawPath;
};

Route compileRoute(const std::string& path, std::function<void(const Request&, Response&)> handler);

Route compileRoute(const std::string& path, std::initializer_list<Middleware> mws,
                   std::function<void(const Request&, Response&)> handler);

} // namespace zuno
