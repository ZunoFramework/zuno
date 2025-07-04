#pragma once

#include <fmt/color.h>
#include <fmt/core.h>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>

namespace zuno::log
{

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
    fmt::color methodColor = fmt::color::cyan;
    fmt::color statusColor;

    if (status < 300)
        statusColor = fmt::color::light_green;
    else if (status < 500)
        statusColor = fmt::color::yellow;
    else
        statusColor = fmt::color::red;

    fmt::print(fg(fmt::color::white) | fmt::emphasis::bold, "[ZUNO] ");
    fmt::print(fg(methodColor), "{}", method);
    fmt::print(" {} ", path);
    fmt::print(fg(fmt::color::gray), "→ ");
    fmt::print(fg(statusColor), "{}", status);
    fmt::print(fg(fmt::color::magenta), " {} ms\n", durationMs);
}

inline void info(const std::string& msg, const auto&... args)
{
    fmt::print(fg(fmt::color::blue) | fmt::emphasis::bold, "[INFO] ");
    fmt::print("{}", timestamp());
    std::string formattedText = fmt::vformat(msg, fmt::make_format_args(args...));
    fmt::print(" {}\n", formattedText);
}

inline void warn(const std::string& msg, const auto&... args)
{
    fmt::print(fg(fmt::color::yellow) | fmt::emphasis::bold, "[WARN] ");
    std::string formattedText = fmt::vformat(msg, fmt::make_format_args(args...));
    fmt::print(fg(fmt::color::gray), "{}", timestamp());
    fmt::print(" {}\n", formattedText);
}

inline void error(const std::string& msg, const auto&... args)
{
    fmt::print(fg(fmt::color::red) | fmt::emphasis::bold, "[ERROR] ");
    std::string formattedText = fmt::vformat(msg, fmt::make_format_args(args...));
    fmt::print(fg(fmt::color::gray), "{}", timestamp());
    fmt::print(" {}\n", formattedText);
}

} // namespace zuno::log