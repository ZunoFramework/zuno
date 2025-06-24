#define CATCH_CONFIG_MAIN
#include "zuno/route.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Dynamic routes compile correctly")
{
    using namespace zuno;

    auto handler = [](const Request&, Response&) {};
    auto route = compileRoute("/user/:id", handler);

    REQUIRE(route.paramNames.size() == 1);
    REQUIRE(route.paramNames[0] == "id");
    REQUIRE(std::regex_match("/user/123", route.pattern));
}
