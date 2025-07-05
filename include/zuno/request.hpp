// request.hpp
#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include "stream_adapter.hpp"

namespace zuno
{

inline std::string decode_url(const std::string& str)
{
    std::string result;
    char ch;
    int i, ii;
    for (i = 0; i < str.length(); i++)
    {
        if (str[i] == '%')
        {
            sscanf(str.substr(i + 1, 2).c_str(), "%x", &ii);
            ch = static_cast<char>(ii);
            result += ch;
            i += 2;
        }
        else if (str[i] == '+')
        {
            result += ' ';
        }
        else
        {
            result += str[i];
        }
    }
    return result;
}

inline std::unordered_map<std::string, std::string> parse_query_string(const std::string& query_str)
{
    std::unordered_map<std::string, std::string> params;
    std::istringstream stream(query_str);
    std::string pair;

    while (std::getline(stream, pair, '&'))
    {
        auto pos = pair.find('=');
        if (pos != std::string::npos)
        {
            std::string key = pair.substr(0, pos);
            std::string value = pair.substr(pos + 1);
            params[decode_url(key)] = decode_url(value);
        }
        else
        {
            params[decode_url(pair)] = "";
        }
    }

    return params;
}

class Request
{
   public:
    Request(const std::string& path, StreamAdapterPtr stream) : url_(path), stream_(std::move(stream))
    {
        ip_ = stream_->remote_endpoint().address().to_string();
    }

    std::string param(const std::string& name) const
    {
        auto it = params.find(name);
        return it != params.end() ? it->second : "";
    }

    std::string query(const std::string& key) const
    {
        auto all = query();
        auto it = all.find(key);
        return it != all.end() ? it->second : "";
    }

    std::unordered_map<std::string, std::string> query() const
    {
        auto pos = url_.find('?');
        if (pos == std::string::npos) return {};
        std::string query_str = url_.substr(pos + 1);
        return parse_query_string(query_str);
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

    std::string url() const
    {
        return url_;
    }
    void setUrl(std::string url)
    {
        url_ = std::move(url);
    }

    std::string path() const
    {
        std::string path = url_;
        std::size_t queryPos = url_.find('?');
        if (queryPos != std::string::npos)
        {
            path = path.substr(0, queryPos);
        }
        return path;
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
    std::string url_;
    std::string ip_;
    StreamAdapterPtr stream_;
};
} // namespace zuno
