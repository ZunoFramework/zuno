#pragma once
#include <string>
#include "zuno/common.hpp"
#include "zuno/logger.hpp"

namespace zuno
{
inline Middleware cors(const std::string& origin = "*")
{
    log::info("CORS Middleware Activated!");

    return [=](Request& req, Response& res, Next next)
    {
        res.setHeader("Access-Control-Allow-Origin", origin);
        res.setHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, PATCH");
        res.setHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");

        if (req.method == "OPTIONS")
        {
            return res.status(204).send("");
        }

        next();
    };
}
} // namespace zuno