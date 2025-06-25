#pragma once
#include <string>
#include "zuno/common.hpp"
#include "zuno/logger.hpp"

namespace zuno
{
inline Middleware cors(const std::string& origin = "*", const std::string& methods = "GET, POST, PUT, DELETE, OPTIONS, PATCH",
                       const std::string& headers = "Content-Type, Authorization")
{
    log::info("CORS Extension Activated!");

    return [=](Request& req, Response& res, Next next)
    {
        res.setHeader("Access-Control-Allow-Origin", origin);
        res.setHeader("Access-Control-Allow-Methods", methods);
        res.setHeader("Access-Control-Allow-Headers", headers);

        if (req.method == "OPTIONS")
        {
            return res.status(204).send("");
        }

        next();
    };
}
} // namespace zuno