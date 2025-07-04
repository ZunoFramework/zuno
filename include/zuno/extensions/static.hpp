#pragma once
#include <filesystem>
#include <fstream>
#include "zuno/common.hpp"

namespace zuno
{

struct StaticFilesOptions
{
    std::string directory;
    std::string urlPrefix = "/";
    bool cache = false;
    int maxAge = 3600; // Default max-age in seconds (1 hour)
};

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

inline Middleware staticFiles(const StaticFilesOptions& options)
{
    log::info("Static Serve Extension Activated!");
    namespace fs = std::filesystem;

    return [=](Request& req, Response& res, Next next)
    {
        std::string path = req.path();

        if (!path.starts_with(options.urlPrefix))
        {
            return next();
        }

        std::string relPath = path.substr(options.urlPrefix.size());
        if (relPath.empty()) relPath = "index.html";

        fs::path filePath = fs::path(options.directory) / relPath;

        if (!fs::exists(filePath) || !fs::is_regular_file(filePath))
        {
            return next();
        }

        // Check if the file has been modified since last served
        auto lastModifiedTime = fs::last_write_time(filePath);
        std::string cacheKey = filePath.string();
        static std::unordered_map<std::string, std::pair<time_t, std::string>> fileCache;

        if (fileCache.find(cacheKey) != fileCache.end() && fileCache[cacheKey].first == lastModifiedTime.time_since_epoch().count())
        {
            res.setHeader("Content-Type", mimeType(filePath.extension().string()));
            if (options.cache)
            {
                res.setHeader("Cache-Control", "public, max-age=" + std::to_string(options.maxAge));
            }
            res.send(fileCache[cacheKey].second);
            return;
        }

        // Read the file from disk
        std::ifstream file(filePath, std::ios::binary);
        if (!file)
        {
            return res.status(500).send("Error reading file");
        }

        std::string body((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        res.setHeader("Content-Type", mimeType(filePath.extension().string()));

        if (options.cache)
        {
            res.setHeader("Cache-Control", "public, max-age=" + std::to_string(options.maxAge));
        }

        // Store the file content in cache
        fileCache[cacheKey] = {lastModifiedTime.time_since_epoch().count(), body};

        res.send(body);
    };
}

} // namespace zuno