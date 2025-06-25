#pragma once
#include <filesystem>
#include <fstream>
#include "zuno/common.hpp"

namespace zuno
{

inline std::string mimeType(const std::string& ext)
{
    if (ext == ".html") return "text/html";
    if (ext == ".css") return "text/css";
    if (ext == ".js") return "application/javascript";
    if (ext == ".png") return "image/png";
    if (ext == ".jpg" || ext == ".jpeg") return "image/jpeg";
    if (ext == ".svg") return "image/svg+xml";
    return "application/octet-stream";
}

inline Middleware staticFiles(const std::string& directory, const std::string& urlPrefix = "/")
{
    namespace fs = std::filesystem;

    return [=](Request& req, Response& res, Next next)
    {
        std::string path = req.path;

        // Si no coincide con el prefijo, pasar al siguiente middleware
        if (!path.starts_with(urlPrefix))
        {
            return next();
        }

        std::string relPath = path.substr(urlPrefix.size());
        if (relPath.empty()) relPath = "index.html";

        fs::path filePath = fs::path(directory) / relPath;

        if (!fs::exists(filePath) || !fs::is_regular_file(filePath))
        {
            return next();
        }

        std::ifstream file(filePath, std::ios::binary);
        if (!file)
        {
            return res.status(500).send("Error reading file");
        }

        std::string body((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        res.setHeader("Content-Type", mimeType(filePath.extension().string()));
        res.send(body);
    };
}

} // namespace zuno
