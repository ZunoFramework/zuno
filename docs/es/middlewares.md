# Middlewares en Zuno

Como usar middlewares globales y por ruta en zuno:

## Globales

```cpp
app.use([](Request& req, Response& res, Next next) {
    std::cout << "Request: " << req.method() << " " << req.path() << "\n";
    next();
});
```

## Por ruta

```cpp
app.get("/secure", {
    [](Request& req, Response& res, Next next) {
        if (req.headers["Authorization"] != "Token123") {
            return res.status(401).json({ {"error", "Unauthorized"} });
        }
        next();
    }
}, [](Request& req, Response& res) {
    res.send("Acceso concedido");
});
```