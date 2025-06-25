cmake_minimum_required(VERSION 3.20)
project(zuno VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(ZUNO_ENABLE_TESTS ON)
set(ZUNO_BUILD_EXAMPLES ON)

# ---📌 Version Info------------------------------------
add_compile_definitions(
  ZUNO_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
  ZUNO_VERSION_MINOR=${PROJECT_VERSION_MINOR}
  ZUNO_VERSION_PATCH=${PROJECT_VERSION_PATCH}
)

if(ZUNO_ENABLE_TESTS)
  add_compile_definitions(ZUNO_ENABLE_TESTS)
endif()

# ---📦 Dependencies------------------------------------
include(FetchContent)

## Catch2 (testing)
if(ZUNO_ENABLE_TESTS)
  FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.3
    DOWNLOAD_EXTRACT_TIMESTAMP true
  )
  FetchContent_MakeAvailable(catch2)
endif()

## nlohmann::json
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(
  brotli
  GIT_REPOSITORY https://github.com/google/brotli.git
  GIT_TAG        v1.1.0
)
FetchContent_MakeAvailable(brotli)

# ---📁 Source Files------------------------------------
file(GLOB_RECURSE ZUNO_SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE ZUNO_HEADERS CONFIGURE_DEPENDS include/*.hpp)

# ---🎯 Build Targets-----------------------------------
add_library(zuno STATIC ${ZUNO_SOURCES} ${ZUNO_HEADERS})

target_include_directories(zuno PUBLIC include)
target_compile_definitions(zuno PUBLIC ASIO_STANDALONE)
target_link_libraries(zuno PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(zuno PRIVATE brotlienc)


# ---🧪 Unit Tests--------------------------------------
if(ZUNO_ENABLE_TESTS)
  add_executable(zuno-tests
    tests/route_test.cpp
  )

  target_link_libraries(zuno-tests PRIVATE zuno Catch2::Catch2WithMain)
endif()

# ---🚀 Build Example-----------------------------------
if(ZUNO_BUILD_EXAMPLES)
  add_executable(zuno-example examples/main.cpp)
  target_link_libraries(zuno-example PRIVATE zuno)
endif()
