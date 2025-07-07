#pragma once
#include <nlohmann/json.hpp>
#include <string>

namespace zuno
{

using json = nlohmann::json;

inline std::string statusText(int code)
{
    switch (code)
    {
        case 200:
            return "OK";
        case 201:
            return "Created";
        case 204:
            return "No Content";
        case 301:
            return "Moved Permanently";
        case 302:
            return "Found";
        case 400:
            return "Bad Request";
        case 401:
            return "Unauthorized";
        case 403:
            return "Forbidden";
        case 404:
            return "Not Found";
        case 405:
            return "Method Not Allowed";
        case 500:
            return "Internal Server Error";
        case 502:
            return "Bad Gateway";
        case 503:
            return "Service Unavailable";
        default:
            return "Unknown";
    }
}

inline std::string json_to_xml(const json& j, const std::string& root = "response")
{
    std::ostringstream oss;

    std::function<void(const json&, const std::string&)> to_xml;
    to_xml = [&](const json& value, const std::string& tag)
    {
        if (value.is_object())
        {
            oss << "<" << tag << ">";
            for (auto& [key, val] : value.items())
            {
                to_xml(val, key);
            }
            oss << "</" << tag << ">";
        }
        else if (value.is_array())
        {
            for (const auto& item : value)
            {
                to_xml(item, tag); // Reuse same tag for array items
            }
        }
        else
        {
            oss << "<" << tag << ">" << value.dump() << "</" << tag << ">";
        }
    };

    to_xml(j, root);
    return oss.str();
}
} // namespace zuno
