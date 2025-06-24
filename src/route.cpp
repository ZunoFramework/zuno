#include "zuno/route.hpp"

namespace zuno
{

Route compileRoute(const std::string& path, std::function<void(const Request&, Response&)> handler)
{
    std::string regexStr = "^";
    std::vector<std::string> paramNames;

    size_t i = 0;
    while (i < path.size())
    {
        if (path[i] == ':' && i + 1 < path.size())
        {
            size_t j = i + 1;
            while (j < path.size() && path[j] != '/' && path[j] != ':') j++;
            paramNames.push_back(path.substr(i + 1, j - i - 1));
            regexStr += "([^/]+)";
            i = j;
        }
        else
        {
            if (std::ispunct(path[i]) && path[i] != '/')
            {
                regexStr += "\\";
            }
            regexStr += path[i++];
        }
    }
    regexStr += "$";
    return Route{
        std::regex(regexStr), paramNames, handler, {}, path,
    };
}

Route compileRoute(const std::string& path, std::initializer_list<Middleware> mws, std::function<void(const Request&, Response&)> handler)
{
    std::string regexStr = "^";
    std::vector<std::string> paramNames;

    size_t i = 0;
    while (i < path.size())
    {
        if (path[i] == ':' && i + 1 < path.size())
        {
            size_t j = i + 1;
            while (j < path.size() && path[j] != '/' && path[j] != ':') j++;
            paramNames.push_back(path.substr(i + 1, j - i - 1));
            regexStr += "([^/]+)";
            i = j;
        }
        else
        {
            if (std::ispunct(path[i]) && path[i] != '/')
            {
                regexStr += "\\";
            }
            regexStr += path[i++];
        }
    }
    regexStr += "$";

    return Route{
        std::regex(regexStr), paramNames, handler, std::vector<Middleware>(mws), path,
    };
}

} // namespace zuno
