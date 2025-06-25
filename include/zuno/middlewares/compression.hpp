#pragma once
#include <brotli/encode.h>
#include <sstream>
#include "zuno/common.hpp"

namespace zuno
{

inline std::string brotliCompress(const std::string& input)
{
    std::string output;
    size_t maxSize = BrotliEncoderMaxCompressedSize(input.size());
    output.resize(maxSize);

    size_t encodedSize = maxSize;
    auto success = BrotliEncoderCompress(BROTLI_DEFAULT_QUALITY, BROTLI_DEFAULT_WINDOW, BROTLI_MODE_GENERIC, input.size(),
                                         reinterpret_cast<const uint8_t*>(input.data()), &encodedSize, reinterpret_cast<uint8_t*>(&output[0]));

    if (!success) return {};
    output.resize(encodedSize);
    return output;
}

inline Middleware compression(double minRatio = 0.8)
{
    return [=](Request& req, Response& res, Next next)
    {
        res.wrapSend(
            [&res, &req, &minRatio](const std::string& body, auto originalSend)
            {
                if (req.headers.at("accept-encoding").find("br") == std::string::npos)
                {
                    return originalSend(body);
                }

                if (body.size() < 256)
                {
                    return originalSend(body);
                }

                std::string compressed = brotliCompress(body);
                if (compressed.empty() || double(compressed.size()) / body.size() > minRatio)
                {
                    return originalSend(body);
                }

                res.setHeader("Content-Encoding", "br");
                res.setHeader("Content-Length", std::to_string(compressed.size()));
                return originalSend(compressed);
            });

        next();
    };
}

} // namespace zuno
