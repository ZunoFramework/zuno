cmake_minimum_required(VERSION 3.20)
project(zuno VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/version.cmake)
include(cmake/options.cmake)
include(cmake/dependencies.cmake)

# ---📁 Source Files------------------------------------
file(GLOB_RECURSE ZUNO_SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE ZUNO_HEADERS CONFIGURE_DEPENDS include/*.hpp)

# ---🎯 Build Targets-----------------------------------
add_library(zuno STATIC ${ZUNO_SOURCES} ${ZUNO_HEADERS})

target_include_directories(zuno PUBLIC include)
target_compile_definitions(zuno PUBLIC ASIO_STANDALONE)
target_link_libraries(zuno PUBLIC nlohmann_json::nlohmann_json brotlienc fmt::fmt)
target_link_libraries(zuno PRIVATE ssl crypto)

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

# ---📦 Install Targets---------------------------------
install(TARGETS zuno
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)

install(FILES ${ZUNO_HEADERS}
  DESTINATION include/zuno
)