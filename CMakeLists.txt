cmake_minimum_required(VERSION 3.14)
project(Zuno LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_library(zuno STATIC
    src/zuno.cpp
)

add_executable(zuno_example examples/main.cpp)
target_link_libraries(zuno_example zuno)
