#pragma once

#include <chrono>
#include <unordered_map>
#include "zuno/common.hpp"
#include "zuno/logger.hpp"

namespace zuno
{

struct RateLimitOptions
{
    size_t max = 100;           // Max Rate
    size_t intervalMs = 60'000; // In MS
};

Middleware rateLimit(const RateLimitOptions& opts)
{
    log::info("Rate-Limit Extension Activated!");
    using namespace std::chrono;
    static std::unordered_map<std::string, std::pair<int, steady_clock::time_point>> counters;

    return [opts](Request& req, Response& res, Next next)
    {
        auto it = req.headers.find("x-forwarded-for");
        std::string key = (it != req.headers.end()) ? it->second : req.ip();

        auto now = steady_clock::now();

        auto& entry = counters[key];
        auto& count = entry.first;
        auto& start = entry.second;

        if (start.time_since_epoch().count() == 0)
        {
            start = now;
        }

        auto elapsed = duration_cast<milliseconds>(now - start).count();

        if (elapsed > opts.intervalMs)
        {
            count = 0;
            start = now;
        }

        if (++count > opts.max)
        {
            res.status(429).setHeader("Retry-After", std::to_string(opts.intervalMs / 1000)).send("Too Many Requests");
            return;
        }

        next();
    };
}

} // namespace zuno