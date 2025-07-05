# ⚡ Zuno Framework

Zuno Framework is a powerful and modern web framework written in C++, designed to deliver exceptional performance without sacrificing simplicity. Inspired by the philosophy of Express.js and powered by standalone Asio, Zuno enables developers to build secure, modular, and scalable web applications with a clean and expressive API.

## 🚀 Key Features

- 🔧 **Fully modular**: Structure your application as a set of independent, reusable modules.  
- 🔒 **TLS support via `useTLS`**: Production-ready HTTPS with a simple and direct function call.  
- ⚙️ **Built on standalone Asio**: Harness the power of asynchronous I/O with zero external dependencies.  
- 🧩 **Intuitive routing**: Define routes with clarity and full control.  
- 🛡️ **Global and per-route middleware**: Chain custom logic for authentication, logging, validation, and more.  
- 📦 **Lightweight and dependency-free**: Ideal for embedded systems or high-performance environments.  

## 🛠️ Quick Example

```cpp
#include <zuno/zuno.hpp>
#include <zuno/logger.hpp>

int main() {
    zuno::App app;

    app.use([](const zuno::Request& req, zuno::Response& res, zuno::Next next) {
        zuno::log::info("Global middleware");
        next();
    });

    app.get("/hello", [](const zuno::Request& req, zuno::Response& res) {
        res.send("Hello from HTTPS Zuno!");
    });

    app.useTLS({"server.crt", "server.key"});

    app.listen(443);
}
```

## 📚 Documentation

Full documentation is coming soon. In the meantime, here are some key components:

- `zuno::App` – Core of the framework  
- `zuno::Request` and `zuno::Response` – Request and response handling  
- `zuno::Next` – Middleware flow control  
- `useTLS(app, config)` – Enable HTTPS with certificate configuration  

## 🧪 Project Status

Zuno Framework is under active development. The following features are available or in progress:

| Feature                | Status           |
|------------------------|------------------|
| Basic routing          | ✅ Ready          |
| TLS via `useTLS`       | ✅ Ready          |
| Global middleware      | ✅ Ready          |
| Per-route middleware   | ✅ Ready          |
| HTML templating        | 🔜 Planned        |
| WebSockets             | 🔜 Planned        |

## 🤝 Contributing

Contributions are welcome! If you'd like to get involved, check out the [issues](https://github.com/ZunoFramework/zuno/issues) or open a pull request.

## 📄 License

Zuno Framework is licensed under the [GNU Affero General Public License v3.0](LICENSE).

---

Crafted with passion by **Seobryn** ⚙️
