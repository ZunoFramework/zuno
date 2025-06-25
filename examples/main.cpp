#include <zuno/middlewares/cors.hpp>
#include <zuno/middlewares/static.hpp>
#include <zuno/zuno.hpp>

int main()
{
    zuno::App app;

    app.use(zuno::cors());
    app.use(zuno::staticFiles("public"));

    app.get("/test/*", [](const zuno::Request& req, zuno::Response& res) { res.send("Recibido " + req.param("*")); });

    app.get("/hola/:name", [](const zuno::Request& req, zuno::Response& res) { res.send("Welcome to Zuno " + req.param("name") + " 🚀"); });

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

    app.listen(3456);
}
