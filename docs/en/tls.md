# Setting Up TLS Connection in Zuno

Zuno provides support for setting up a secure HTTP server using TLS (Transport Layer Security). This guide will walk you through the process of configuring your Zuno application to use TLS.

## Prerequisites

Before proceeding, ensure that you have the following installed:
- OpenSSL library

## Step 1: Generate SSL Certificates

You need to generate an SSL certificate and key file. You can use OpenSSL for this purpose. Run the following commands in your terminal:

```bash
# Generate a private key
openssl genpkey -algorithm RSA -out server.key -aes256

# Generate a self-signed certificate
openssl req -new -x509 -key server.key -out server.crt -days 365
```

## Step 2: Configure Zuno to Use TLS

To configure your Zuno application to use TLS, you need to modify the `main.cpp` file. Here is an example of how to do it:

```cpp
#include "zuno/zuno.hpp"

int main() {
    zuno::App app;

    app.useTLS({"path/to/cert/server.crt", "path/to/cert/server.key"});

    // Register a route
    app.get("/hello", [](const zuno::Request& req, zuno::Response& res) {
        res.send("Hello from Zuno!");
    });

    // Start the server with TLS
    app.listen(443);

    return 0;
}
```

## Step 3: Compile and Run

Compile your application using CMake:

```bash
mkdir build && cd build
cmake ..
make
```

Run the compiled server:

```bash
./server
```

Now, you can access your secure HTTP server at `https://localhost/hello`.

## Troubleshooting

- **Certificate and Key Files**: Ensure that the paths to the certificate and key files are correct.
- 
- **Port 443**: Port 443 is commonly used for HTTPS. If you encounter any issues, try using a different port.

By following these steps, you should be able to set up a secure HTTP server in your Zuno application using TLS.