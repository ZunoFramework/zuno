#pragma once
#include <functional>
#include <regex>
#include <string>
#include <vector>
#include "request.hpp"
#include "response.hpp"

namespace zuno
{

using Middleware = std::function<void(Request&, Response&, Next)>;
using Handler = std::function<void(Request&, Response&)>;

struct Route
{
    std::regex pattern;
    std::vector<std::string> paramNames;
    std::function<void(const Request&, Response&)> handler;
    std::string rawPath;
};

Route compileRoute(const std::string& path, std::function<void(const Request&, Response&)> handler);

} // namespace zuno
