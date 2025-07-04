#include "zuno/app.hpp"
#include <fmt/color.h>
#include <fmt/core.h>
#include <algorithm>
#include <asio.hpp>
#include "zuno/http_server.hpp"
#include "zuno/logger.hpp"
#include "zuno/route.hpp"

namespace zuno
{

void App::addRoute(const std::string& method, const std::string& path, Handler handler)
{
    std::string m = method;
    std::transform(m.begin(), m.end(), m.begin(), ::toupper);
    routes[m].push_back(compileRoute(path, handler));
}

void App::addRoute(const std::string& method, const std::string& path, std::initializer_list<Middleware> mws, Handler handler)
{
    std::string m = method;
    std::transform(m.begin(), m.end(), m.begin(), ::toupper);
    routes[m].push_back(compileRoute(path, mws, handler));
}

void App::get(const std::string& path, Handler handler)
{
    addRoute("GET", path, std::move(handler));
}

void App::post(const std::string& path, Handler handler)
{
    addRoute("POST", path, std::move(handler));
}

void App::put(const std::string& path, Handler handler)
{
    addRoute("PUT", path, std::move(handler));
}

void App::patch(const std::string& path, Handler handler)
{
    addRoute("PATCH", path, std::move(handler));
}

void App::del(const std::string& path, Handler handler)
{
    addRoute("DELETE", path, std::move(handler));
}

void App::options(const std::string& path, Handler handler)
{
    addRoute("OPTIONS", path, std::move(handler));
}

// With Midlewares

void App::get(const std::string& path, std::initializer_list<Middleware> mws, Handler handler)
{
    addRoute("GET", path, mws, std::move(handler));
}

void App::post(const std::string& path, std::initializer_list<Middleware> mws, Handler handler)
{
    addRoute("POST", path, mws, std::move(handler));
}

void App::put(const std::string& path, std::initializer_list<Middleware> mws, Handler handler)
{
    addRoute("PUT", path, mws, std::move(handler));
}

void App::patch(const std::string& path, std::initializer_list<Middleware> mws, Handler handler)
{
    addRoute("PATCH", path, mws, std::move(handler));
}

void App::del(const std::string& path, std::initializer_list<Middleware> mws, Handler handler)
{
    addRoute("DELETE", path, mws, std::move(handler));
}

void App::options(const std::string& path, std::initializer_list<Middleware> mws, Handler handler)
{
    addRoute("OPTIONS", path, mws, std::move(handler));
}

std::function<void(Request&, Response&)> App::resolveHandler(const std::string& method, const std::string& path,
                                                             std::unordered_map<std::string, std::string>& outParams) const
{
    auto it = routes.find(method);
    if (it == routes.end()) return nullptr;

    for (const auto& route : it->second)
    {
        std::smatch match;
        if (std::regex_match(path, match, route.pattern))
        {
            for (size_t i = 0; i < route.paramNames.size(); ++i)
            {
                outParams[route.paramNames[i]] = match[i + 1];
            }

            return [route](Request& req, Response& res)
            {
                std::size_t index = 0;
                std::function<void()> next;

                next = [&]()
                {
                    if (index < route.middlewares.size())
                    {
                        auto& mw = route.middlewares[index++];
                        mw(req, res, next);
                    }
                    else
                    {
                        route.handler(req, res);
                    }
                };

                next();
            };
        }
    }

    return nullptr;
}

void App::listen(int port)
{
    try
    {
        fmt::print(fg(fmt::color::magenta) | fmt::emphasis::bold, "Zuno v{}\n", ZUNO_VERSION_STR);

        asio::io_context ctx;
        HttpServer server(ctx, port, *this);
        server.start();

        fmt::print(fg(fmt::color::white) | fmt::emphasis::bold, "[ZUNO] ");
        fmt::print("🚀 Listening on: ");
        fmt::print(fg(fmt::color::cyan) | fmt::emphasis::bold, "http://localhost:{}\n", port);

        ctx.run();
    }
    catch (std::system_error err)
    {
        log::error("{}", err.what());
    }
}

} // namespace zuno