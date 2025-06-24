cmake_minimum_required(VERSION 3.14)
project(Zuno LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)

add_library(zuno STATIC
    src/app.cpp
    src/http_server.cpp
    src/route.cpp
)

add_executable(zuno_example examples/main.cpp)

include(FetchContent)

FetchContent_Declare(
  asio
  GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
  GIT_TAG asio-1-30-1
)

FetchContent_MakeAvailable(asio)
target_compile_definitions(zuno PUBLIC ASIO_STANDALONE)

target_link_libraries(zuno_example zuno)
