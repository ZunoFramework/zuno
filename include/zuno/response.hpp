#pragma once

#include <nlohmann/json.hpp>
#include <ostream>
#include <string>
#include <unordered_map>
#include "zuno/stream_adapter.hpp"
#include "zuno/utils.hpp"
#include "zuno/version.hpp"

namespace zuno
{

using SendFunc = std::function<void(const std::string&)>;
using SendWrapper = std::function<void(const std::string&, SendFunc)>;

class Response
{
   public:
    explicit Response(StreamAdapterPtr stream) : stream_(std::move(stream)), buffer_(), ostream_(&buffer_) {}

    void send(const std::string& body)
    {
        body_ = body;
        auto doSend = [this](const std::string& finalBody)
        {
            ostream_ << statusLine();

            for (const auto& [key, value] : headers_)
            {
                ostream_ << key << ": " << value << "\r\n";
            }

            ostream_ << "Content-Length: " << finalBody.size() << "\r\n"
                     << "\r\n"
                     << finalBody;

            flush();
        };

        if (sendWrapper_)
        {
            sendWrapper_(body, doSend);
        }
        else
        {
            doSend(body);
        }
    }

    void wrapSend(SendWrapper wrapper)
    {
        sendWrapper_ = std::move(wrapper);
    }

    void redirect(const std::string& url, int status = 302)
    {
        statusCode_ = status;
        setHeader("Location", url);
        send("");
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

    std::string body() const
    {
        return body_;
    }

    std::unordered_map<std::string, std::string> headers() const
    {
        return headers_;
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

    void write(const std::string& chunk)
    {
        ostream_ << chunk;
    }

    void end()
    {
        flush();
    }

   private:
    StreamAdapterPtr stream_;
    asio::streambuf buffer_;
    std::ostream ostream_;
    std::string body_;
    std::unordered_map<std::string, std::string> headers_ = {{"X-Powered-By", std::string("Zuno/") + ZUNO_VERSION_STR},
                                                             {"Content-Type", "text/plain;charset=utf-8"}};

    SendWrapper sendWrapper_ = nullptr;
    bool headersSent_ = false;
    int statusCode_ = 200;

    std::string statusLine()
    {
        return "HTTP/1.1 " + std::to_string(statusCode_) + " " + statusText() + "\r\n";
    }

    void flush()
    {
        stream_->write(asio::buffer_cast<const char*>(buffer_.data()), buffer_.size());
        buffer_.consume(buffer_.size());
    }
};

} // namespace zuno
