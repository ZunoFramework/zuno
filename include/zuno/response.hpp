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

    void xml(const nlohmann::json& data)
    {
        setHeader("Content-Type", "application/xml");
        send(json_to_xml(data));
    }

    void html(const std::string& file_path)
    {
        setHeader("Content-Type", "text/html");
        send(readFile(file_path));
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

    Response& setCookie(const std::string& key, const std::string value)
    {
        cookies_[key] = value;
        updateHeaders();
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
    std::unordered_map<std::string, std::string> headers_ = {{"X-Powered-By", std::string("Zuno v.") + ZUNO_VERSION_STR},
                                                             {"Content-Type", "text/plain;charset=utf-8"}};
    std::unordered_map<std::string, std::string> cookies_ = {};

    SendWrapper sendWrapper_ = nullptr;
    bool headersSent_ = false;
    int statusCode_ = 200;

    std::string statusLine()
    {
        return "HTTP/1.1 " + std::to_string(statusCode_) + " " + statusText() + "\r\n";
    }

    void flush()
    {
        auto begin = asio::buffers_begin(buffer_.data());
        auto end = asio::buffers_end(buffer_.data());
        stream_->write(&(*begin), std::distance(begin, end));
        buffer_.consume(buffer_.size());
    }

    void updateHeaders()
    {
        std::stringstream ss;
        for (const auto& pair : cookies_)
        {
            ss << pair.first << "=" << pair.second << "; ";
        }
        if (!ss.str().empty())
        {
            ss.seekp(-2);
        }

        headers_["Set-Cookie"] = ss.str();
    }
};

} // namespace zuno