#pragma once
#include <asio.hpp>
#include <asio/ssl.hpp>
#include <memory>
#include <string>

namespace zuno
{

class StreamAdapter
{
   public:
    virtual ~StreamAdapter() = default;

    virtual asio::ip::tcp::endpoint remote_endpoint() const = 0;

    virtual std::size_t write(const char* data, std::size_t size) = 0;
    virtual std::size_t write(const std::string& data)
    {
        return write(data.data(), data.size());
    }

    virtual std::size_t read_until(asio::streambuf& buffer, const std::string& delim) = 0;
    virtual std::size_t read(asio::streambuf& buffer, std::size_t size) = 0;
};

using StreamAdapterPtr = std::shared_ptr<StreamAdapter>;

} // namespace zuno
