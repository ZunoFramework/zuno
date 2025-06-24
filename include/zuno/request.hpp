#pragma once

#include <string>
#include <unordered_map>
#include <asio.hpp>
#include <nlohmann/json.hpp>

namespace zuno {

class Request {
public:
    Request(const std::string& path, asio::ip::tcp::socket& socket)
        : path(path), socket_(socket) {}
    
    std::unordered_map<std::string, std::string> headers;
    std::string body;

    std::string param(const std::string& name) const {
        auto it = params.find(name);
        return it != params.end() ? it->second : "";
    }

    nlohmann::json json() const {
        return nlohmann::json::parse(body);
    }; 

    std::string path;
    std::unordered_map<std::string, std::string> params;

private:
    asio::ip::tcp::socket& socket_;
};

}