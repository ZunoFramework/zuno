# Middleware System

Zuno supports global and route-specific middleware functions.

## Global middleware

```cpp
app.use([](Request& req, Response& res, Next next) {
    std::cout << "Incoming: " << req.method << " " << req.path << "\n";
    next();
});
```

## Per-route middleware

```cpp
app.get("/secure", {
    [](Request& req, Response& res, Next next) {
        if (req.headers["Authorization"] != "Token123")
            return res.status(401).json({ {"error", "Unauthorized"} });
        next();
    }
}, [](Request& req, Response& res) {
    res.send("Access granted");
});
```

## Middleware chaining

Each middleware must call next() to continue. If omitted, the handler won’t run.