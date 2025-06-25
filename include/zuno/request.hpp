#pragma once

#include <asio.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include "zuno/logger.hpp"

namespace zuno
{

class Request
{
   public:
    Request(const std::string& path, asio::ip::tcp::socket& socket) : path(path), socket_(socket)
    {
        ip = socket_.remote_endpoint().address().to_string();
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
        return nlohmann::json::parse(body);
    };

    std::string body;
    std::string method;
    std::string path;
    std::string ip;
    std::unordered_map<std::string, std::string> headers;
    std::unordered_map<std::string, std::string> params;

   private:
    asio::ip::tcp::socket& socket_;
};

} // namespace zuno