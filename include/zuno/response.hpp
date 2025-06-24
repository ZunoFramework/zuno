#pragma once

#include <asio.hpp>
#include <string>
#include <ostream>
#include <nlohmann/json.hpp>

namespace zuno {

class Response {
public:
    explicit Response(asio::ip::tcp::socket& socket)
        : socket_(socket), stream_(&buffer_) {}

    void send(const std::string& body, int statusCode = 200, const std::string& contentType = "text/plain") {
        stream_ << "HTTP/1.1 " << statusCode << " OK\r\n"
                << "Content-Length: " << body.size() << "\r\n"
                << "Content-Type: " << contentType << "; charset=utf-8\r\n"
                << "\r\n"
                << body;
        flush();
    }

    void json(const nlohmann::json& data, int statusCode = 200) {
        std::string body = data.dump(2); 
        send(body, statusCode, "application/json");
    }

private:
    asio::ip::tcp::socket& socket_;
    asio::streambuf buffer_;
    std::ostream stream_;

    void flush() {
        asio::write(socket_, buffer_);
    }
};

} // namespace zuno
