#pragma once

#include <string>
#include <sstream>
#include <asio.hpp>

namespace zuno {

class Response {
public:
    explicit Response(asio::ip::tcp::socket& socket)
        : socket_(socket) {}

    void send(const std::string& body, int status = 200, const std::string& contentType = "text/plain") {
        std::ostringstream response_stream;
        response_stream << "HTTP/1.1 " << status << " " << statusMessage(status) << "\r\n"
                        << "Content-Length: " << body.size() << "\r\n"
                        << "Content-Type: " << contentType << "; charset=utf-8\r\n"
                        << "Connection: close\r\n"
                        << "\r\n"
                        << body;

        asio::write(socket_, asio::buffer(response_stream.str()));
    }

private:
    asio::ip::tcp::socket& socket_;

    std::string statusMessage(int code) const {
        switch (code) {
            case 200: return "OK";
            case 404: return "Not Found";
            case 500: return "Internal Server Error";
            default: return "Unknown";
        }
    }
};

} // namespace zuno
