#pragma once

#include <vector>
#include "request.hpp"
#include "response.hpp"

namespace zuno
{
using Handler = std::function<void(const Request&, Response&)>;
using Next = std::function<void()>;
using Middleware = std::function<void(Request&, Response&, Next)>;
} // namespace zuno