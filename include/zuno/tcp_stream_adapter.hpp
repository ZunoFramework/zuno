#pragma once
#include "stream_adapter.hpp"

namespace zuno
{

class TcpStreamAdapter : public StreamAdapter
{
   public:
    TcpStreamAdapter(asio::ip::tcp::socket socket) : socket_(std::move(socket)) {}

    asio::ip::tcp::endpoint remote_endpoint() const override
    {
        return socket_.remote_endpoint();
    }

    std::size_t write(const std::string& data) override
    {
        return asio::write(socket_, asio::buffer(data));
    }

    std::size_t write(const char* data, std::size_t size) override
    {
        return asio::write(socket_, asio::buffer(data, size));
    }

    std::size_t read_until(asio::streambuf& buffer, const std::string& delim) override
    {
        return asio::read_until(socket_, buffer, delim);
    }

    std::size_t read(asio::streambuf& buffer, std::size_t size) override
    {
        return asio::read(socket_, buffer, asio::transfer_exactly(size));
    }

   private:
    asio::ip::tcp::socket socket_;
};

} // namespace zuno
