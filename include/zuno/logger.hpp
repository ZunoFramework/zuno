#pragma once
#include <iostream>
#include <string>

namespace zuno::log
{

namespace color
{
const std::string reset = "\033[0m";
const std::string bold = "\033[1m";
const std::string green = "\033[32m";
const std::string yellow = "\033[33m";
const std::string red = "\033[31m";
const std::string cyan = "\033[36m";
const std::string magenta = "\033[35m";
const std::string gray = "\033[90m";
} // namespace color

inline void request(const std::string& method, const std::string& path, int status, long durationMs)
{
    std::string methodColor = color::cyan;
    std::string statusColor;

    if (status < 300)
        statusColor = color::green;
    else if (status < 500)
        statusColor = color::yellow;
    else
        statusColor = color::red;

    std::cout << color::bold << "[ZUNO] " << methodColor << method << " " << color::reset << path
              << " " << color::gray << "→ " << statusColor << status << color::reset << " "
              << color::magenta << durationMs << "ms" << color::reset << "\n";
}

} // namespace zuno::log
