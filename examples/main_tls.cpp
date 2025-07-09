#include <zuno/extensions/compression.hpp>
#include <zuno/extensions/cors.hpp>
#include <zuno/extensions/rate-limit.hpp>
#include <zuno/extensions/static.hpp>
#include <zuno/zuno.hpp>

int main()
{
    zuno::App app;

    app.useTLS({"examples/certs/server.crt", "examples/certs/server.key"});

    app.use(zuno::cors());
    app.use(zuno::staticFiles({"public", "/", true, 36}));
    app.use(zuno::compression());

    app.get("/rate-test", {zuno::rateLimit({10, 30000})}, [](const zuno::Request& req, zuno::Response& res) { res.send("Rate Limit Test"); });

    app.get("/ping",
            [](auto& req, auto& res)
            {
                zuno::log::info("IsSecure? {}", req.isSecure());
                res.send("Pong! 🚀");
            });

    app.post("/echo",
             [](const zuno::Request& req, zuno::Response& res)
             {
                 try
                 {
                     auto body = req.json();
                     res.json(body);
                 }
                 catch (std::runtime_error& ex)
                 {
                     res.status(500).json({{"error", ex.what()}});
                 }
             });

    app.put("/update", [](const zuno::Request& req, zuno::Response& res) { res.send("PUT /update received"); });

    app.del("/delete", [](const zuno::Request& req, zuno::Response& res) { res.send("DELETE /delete received"); });

    app.get("/stream",
            [](const zuno::Request&, zuno::Response& res)
            {
                res.setHeader("Content-Type", "text/plain");

                for (int i = 0; i < 5; ++i)
                {
                    res.write("Chunk " + std::to_string(i) + "\n");
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                }

                res.end();
            });

    app.get("/old-route", [](const zuno::Request&, zuno::Response& res) { res.redirect("/new-route"); });
    app.get("/new-route", [](const zuno::Request&, zuno::Response& res) { res.send("This is the new Route"); });

    app.listen(443);
}
