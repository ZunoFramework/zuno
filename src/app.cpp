#include "zuno/app.hpp"
#include "zuno/http_server.hpp"
#include "zuno/route.hpp"
#include "zuno/logger.hpp"
#include <asio.hpp>
#include <algorithm>

using namespace zuno::log::color;

namespace zuno {

void App::addRoute(const std::string& method, const std::string& path, Handler handler) {
    std::string m = method;
    std::transform(m.begin(), m.end(), m.begin(), ::toupper);
    routes[m].push_back(compileRoute(path, handler));
}


void App::get(const std::string& path, Handler handler)  { addRoute("GET", path, handler); }
void App::post(const std::string& path, Handler handler) { addRoute("POST", path, handler); }
void App::put(const std::string& path, Handler handler)  { addRoute("PUT", path, handler); }
void App::del(const std::string& path, Handler handler)  { addRoute("DELETE", path, handler); }

Handler App::resolveHandler(const std::string& method,
                            const std::string& path,
                            std::unordered_map<std::string, std::string>& outParams) const {
    std::string m = method;
    std::transform(m.begin(), m.end(), m.begin(), ::toupper);

    auto it = routes.find(m);
    if (it == routes.end()) return nullptr;

    for (const auto& route : it->second) {
        std::smatch match;
        if (std::regex_match(path, match, route.pattern)) {
            for (size_t i = 0; i < route.paramNames.size(); ++i) {
                outParams[route.paramNames[i]] = match[i + 1];
            }
            return route.handler;
        }
    }

    return nullptr;
}


void App::listen(int port) {
    asio::io_context ctx;
    HttpServer server(ctx, port, *this);
    server.start();
    std::cout << bold << "[ZUNO] 🚀 Listening on "
          << cyan << "http://localhost:" << port
          << reset << "\n";
    ctx.run();
}

} // namespace zuno
