#include "zuno/route.hpp"

namespace zuno
{

std::pair<std::string, std::vector<std::string>> compilePattern(const std::string& path)
{
    std::string pattern;
    std::vector<std::string> params;

    if (path == "*")
    {
        pattern = ".*";
        params.push_back("*");
    }
    else
    {
        std::ostringstream oss;
        oss << "^";
        std::istringstream iss(path);
        std::string token;

        while (std::getline(iss, token, '/'))
        {
            if (token.empty()) continue;

            if (token[0] == ':')
            {
                oss << "/([^/]+)";
                params.push_back(token.substr(1));
            }
            else if (token == "*")
            {
                oss << "/(.*)";
                params.push_back("*");
                break;
            }
            else
            {
                oss << "/" << token;
            }
        }

        oss << "$";
        pattern = oss.str();
    }

    return {pattern, params};
}

Route compileRoute(const std::string& path, std::function<void(const Request&, Response&)> handler)
{
    auto [pattern, params] = compilePattern(path);

    std::regex re(pattern);

    return Route{
        std::move(re), params, handler, {}, path,
    };
}

Route compileRoute(const std::string& path, std::initializer_list<Middleware> mws, std::function<void(const Request&, Response&)> handler)
{
    auto [pattern, params] = compilePattern(path);
    std::regex re(pattern);
    return Route{
        std::move(re), params, handler, std::vector<Middleware>(mws), path,
    };
}

} // namespace zuno
