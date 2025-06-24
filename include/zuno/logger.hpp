#pragma once

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
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

inline std::string timestamp()
{
    using namespace std::chrono;
    auto now = system_clock::now();
    auto time = system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&time);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%H:%M:%S");
    return oss.str();
}

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

inline void log(const std::string& msg)
{
    std::cout << msg << "\n";
}

inline void info(const std::string& msg)
{
    using namespace color;
    std::cout << bold << cyan << "[INFO] " << gray << "[" << timestamp() << "] " << reset << msg
              << "\n";
}

inline void warn(const std::string& msg)
{
    using namespace color;
    std::cout << bold << yellow << "[WARN] " << gray << "[" << timestamp() << "] " << reset << msg
              << "\n";
}

inline void error(const std::string& msg)
{
    using namespace color;
    std::cerr << bold << red << "[ERROR] " << gray << "[" << timestamp() << "] " << reset << msg
              << "\n";
}

} // namespace zuno::log
