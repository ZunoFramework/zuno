#pragma once

#include <asio.hpp>
#include <nlohmann/json.hpp>
#include <ostream>
#include <string>
#include "zuno/utils.hpp"
#include "zuno/version.hpp"

namespace zuno
{

class Response
{
   public:
    explicit Response(asio::ip::tcp::socket& socket) : socket_(socket), stream_(&buffer_) {}

    void send(const std::string& body)
    {
        stream_ << "HTTP/1.1 " << statusCode_ << " " << zuno::statusText(statusCode_) << "\r\n";

        for (const auto& [key, value] : headers_)
        {
            stream_ << key << ": " << value << "\r\n";
        }

        stream_ << "Content-Length: " << body.size() << "\r\n"
                << "charset=utf-8\r\n"
                << "\r\n"
                << body;
        flush();
    }

    void json(const nlohmann::json& data)
    {
        std::string body = data.dump(2);
        setHeader("Content-Type", "application/json");
        send(body);
    }

    int statusCode() const
    {
        return statusCode_;
    }

    std::string statusText() const
    {
        return zuno::statusText(statusCode_);
    }

    Response& status(int code)
    {
        statusCode_ = code;
        return *this;
    }

    Response& setHeader(const std::string& key, const std::string& value)
    {
        headers_[key] = value;
        return *this;
    }

   private:
    asio::ip::tcp::socket& socket_;
    asio::streambuf buffer_;
    std::ostream stream_;
    std::unordered_map<std::string, std::string> headers_ = {{"X-Powered-By", std::string("Zuno/") + ZUNO_VERSION_STR},
                                                             {"Content-Type", "text/plain"}};

    int statusCode_ = 200;

    void flush()
    {
        asio::write(socket_, buffer_);
    }
};

} // namespace zuno
