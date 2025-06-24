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


    std::string param(const std::string& name) const {
        auto it = params.find(name);
        return it != params.end() ? it->second : "";
    }

    nlohmann::json json() const {
        if (headers.at("Content-Type") != "application/json") {
            throw std::runtime_error("Unsupported Content-Type");
        }
        return nlohmann::json::parse(body);
    }; 

    std::string body;
    std::string method;
    std::string path;
    std::unordered_map<std::string, std::string> headers;
    std::unordered_map<std::string, std::string> params;

private:
    asio::ip::tcp::socket& socket_;
};

}