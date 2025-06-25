#include "zuno/response.hpp"
#include <string>

namespace zuno
{

void Response::write(const std::string& chunk)
{
    if (!headersSent_)
    {
        std::string response = statusLine() + headersToString() + "\r\n";
        asio::write(socket_, asio::buffer(response));
        headersSent_ = true;
    }
    asio::write(socket_, asio::buffer(chunk));
}

void Response::end()
{
    asio::write(socket_, asio::buffer("\r\n"));
    socket_.shutdown(asio::ip::tcp::socket::shutdown_send);
}

std::string Response::statusLine()
{
    return "HTTP/1.1 " + std::to_string(statusCode_) + " " + statusText() + "\r\n";
}

std::string Response::headersToString()
{
    std::string result;
    for (const auto& [key, value] : headers_)
    {
        result += key + ": " + value + "\r\n";
    }
    return result;
}

void Response::redirect(const std::string& url, int status)
{
    statusCode_ = status;
    setHeader("Location", url);
    setHeader("Content-Type", "text/plain");
    send("Redirecting to " + url);
}

void Response::wrapSend(SendWrapper wrapper)
{
    sendWrapper_ = std::move(wrapper);
}
} // namespace zuno