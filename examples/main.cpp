#include <zuno/zuno.hpp>

int main()
{
    zuno::App app;

    app.get("/",
            [](const zuno::Request& req, zuno::Response& res) { res.send("Welcome to Zuno 🚀"); });

    app.get("/hola/:name", [](const zuno::Request& req, zuno::Response& res)
            { res.send("Welcome to Zuno " + req.param("name") + " 🚀"); });

    app.get("/ping", [](const zuno::Request& req, zuno::Response& res) { res.send("pong"); });

    app.post("/login", [](const zuno::Request& req, zuno::Response& res)
             { res.send("POST /login received"); });

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

    app.get("/secret",
            {[](zuno::Request& req, zuno::Response& res, zuno::Next next)
             {
                 const auto& token = req.headers["Authorization"];
                 if (token != "Token123")
                 {
                     return res.status(401).json({{"error", "Token inválido"}});
                 }
                 next();
             }},
            [](const zuno::Request& req, zuno::Response& res)
            { res.json({{"secret", "Aquí tienes el acceso 🤫"}}); });

    app.put("/update", [](const zuno::Request& req, zuno::Response& res)
            { res.send("PUT /update received"); });

    app.del("/delete", [](const zuno::Request& req, zuno::Response& res)
            { res.send("DELETE /delete received"); });

    app.listen(3000);
}
