#pragma once
#include "stream_adapter.hpp"

namespace zuno
{

class TlsStreamAdapter : public StreamAdapter
{
   public:
    TlsStreamAdapter(std::shared_ptr<asio::ssl::stream<asio::ip::tcp::socket>> stream) : stream_(std::move(stream)) {}

    asio::ip::tcp::endpoint remote_endpoint() const override
    {
        return stream_->lowest_layer().remote_endpoint();
    }

    std::size_t write(const std::string& data) override
    {
        return asio::write(*stream_, asio::buffer(data));
    }

    std::size_t write(const char* data, std::size_t size) override
    {
        return asio::write(*stream_, asio::buffer(data, size));
    }

    std::size_t read_until(asio::streambuf& buffer, const std::string& delim) override
    {
        return asio::read_until(*stream_, buffer, delim);
    }

    std::size_t read(asio::streambuf& buffer, std::size_t size) override
    {
        return asio::read(*stream_, buffer, asio::transfer_exactly(size));
    }

   private:
    std::shared_ptr<asio::ssl::stream<asio::ip::tcp::socket>> stream_;
};

} // namespace zuno
