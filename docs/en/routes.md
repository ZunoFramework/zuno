# Routing in Zuno

Zuno provides expressive route handling with dynamic path support.

## Basic route registration

```cpp
app.get("/ping", [](const Request& req, Response& res) {
    res.send("pong");
});
```

## Dynamic route parameters

```cpp
app.get("/users/:id", [](const Request& req, Response& res) {
    auto id = req.params["id"];
    res.json({ {"userId", id} });
});
```

## Supported HTTP methods

- GET, POST, PUT, PATCH, DEL, etc.