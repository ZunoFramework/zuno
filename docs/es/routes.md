# Rutas en Zuno

## Registrar rutas

```cpp
app.get("/ping", [](const Request& req, Response& res) {
    res.send("pong");
});
```

## Rutas dinámicas

```cpp
app.get("/users/:id", [](const Request& req, Response& res) {
    const auto& id = req.params["id"];
    res.json({ {"userId", id} });
});
```

## Métodos soportados

- GET, POST, PUT, DEL, PATCH, etc.
