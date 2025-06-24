## 🚀 Getting Started with Zuno
Zuno is a lightweight, modern HTTP server framework written in C++. This guide walks you through integrating Zuno into your own CMake-based project from scratch.

## 📦 1. Add Zuno to Your Project
- Option A – As a submodule (recommended for contributors)
```bash
git submodule add https://github.com/your-org/zuno.git external/zuno
```

Then in your root CMakeLists.txt:

```cmake
add_subdirectory(external/zuno)
target_link_libraries(your_app PRIVATE zuno)
```

Option B – As a FetchContent dependency
```cmake
include(FetchContent)

FetchContent_Declare(
  zuno
  GIT_REPOSITORY https://github.com/your-org/zuno.git
  GIT_TAG main
)

FetchContent_MakeAvailable(zuno)
target_link_libraries(your_app PRIVATE zuno)
```

## 🛠 2. Minimal Example (main.cpp)
```cpp
#include "zuno/app.hpp"

int main() {
    zuno::App app;

    app.get("/hello", [](const zuno::Request& req, zuno::Response& res) {
        res.send("Hello from Zuno!");
    });

    app.listen(8080);
}
```

Compile and run:

```bash
g++ main.cpp -Iexternal/zuno/include -std=c++20 -o server
./server
```

Then open http://localhost:8080/hello in your browser.

## 🔐 3. Optional Middleware & Routing Features

Zuno supports:

- Global and per-route middleware
- Dynamic routes (/users/:id)
- JSON responses using nlohmann::json
- CORS configuration helpers
- Logging utilities with color and timestamps

Example with middleware:

```cpp
app.get("/secure", {
    [](zuno::Request& req, zuno::Response& res, zuno::Next next) {
        if (req.headers["Authorization"] != "Secret") {
            return res.status(401).send("Unauthorized");
        }
        next();
    }
}, [](const zuno::Request& req, zuno::Response& res) {
    res.send("Welcome!");
});
```

## 🧪 4. Running Tests (Catch2 required)

After setting up Zuno with Catch2, run:

```bash
cmake -Bbuild -H.
cmake --build build --target zuno-tests
./build/zuno-tests
```

## 📁 Folder Structure Recommendation
```
your-project/
├── src/
│   └── main.cpp
├── CMakeLists.txt
└── external/
    └── zuno/
```

## 🧩 Need Help?

Feel free to explore the docs/en/ folder for more detailed API guides and usage tips. Contributions are welcome!