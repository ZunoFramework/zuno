// request.hpp
#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include "stream_adapter.hpp"

namespace zuno
{

class Request
{
   public:
    Request(const std::string& path, StreamAdapterPtr stream) : path_(path), stream_(std::move(stream))
    {
        ip_ = stream_->remote_endpoint().address().to_string();
    }

    std::string param(const std::string& name) const
    {
        auto it = params.find(name);
        return it != params.end() ? it->second : "";
    }

    nlohmann::json json() const
    {
        if (headers.at("Content-Type") != "application/json")
        {
            throw std::runtime_error("Unsupported Content-Type");
        }
        return nlohmann::json::parse(body_);
    }

    std::string body() const
    {
        return body_;
    }
    void setBody(std::string body)
    {
        body_ = std::move(body);
    }

    std::string method() const
    {
        return method_;
    }
    void setMethod(std::string method)
    {
        method_ = std::move(method);
    }

    std::string ip() const
    {
        return ip_;
    }
    void setIp(std::string ip)
    {
        ip_ = std::move(ip);
    }

    std::string path() const
    {
        return path_;
    }
    void setPath(std::string path)
    {
        path_ = std::move(path);
    }

    std::unordered_map<std::string, std::string> headers;
    std::unordered_map<std::string, std::string> params;

    StreamAdapterPtr stream() const
    {
        return stream_;
    }

   private:
    std::string body_;
    std::string method_;
    std::string path_;
    std::string ip_;
    StreamAdapterPtr stream_;
};

} // namespace zuno
