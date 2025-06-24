#pragma once
#include <string>
#include <unordered_map>

namespace zuno {
class Request {
public:
    explicit Request(const std::string& p) : path(p) {}

    std::string param(const std::string& name) const {
        auto it = params.find(name);
        return it != params.end() ? it->second : "";
    }

    std::string path;
    std::unordered_map<std::string, std::string> params;
};
}
