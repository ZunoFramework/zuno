cmake_minimum_required(VERSION 3.20)
project(zuno VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include(cmake/version.cmake)
include(cmake/options.cmake)
include(cmake/dependencies.cmake)

# ---📁 Source Files------------------------------------
file(GLOB_RECURSE ZUNO_SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE ZUNO_HEADERS CONFIGURE_DEPENDS include/*.hpp)

# ---🎯 Build Targets-----------------------------------
add_library(zuno STATIC ${ZUNO_SOURCES} ${ZUNO_HEADERS})

target_include_directories(zuno PUBLIC include ${OPENSSL_INCLUDE_DIR} ${asio_SOURCE_DIR}/asio/include)
target_link_libraries(zuno PUBLIC nlohmann_json::nlohmann_json brotlienc fmt::fmt)
target_link_libraries(zuno PRIVATE ${OPENSSL_LIBRARIES})

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

  add_executable(zuno-example-tls examples/main_tls.cpp)
  target_link_libraries(zuno-example-tls PRIVATE zuno)
endif()

# --- Build Tools------------------------------------------
add_executable(print_version tools/print_version.cpp)
target_include_directories(print_version PRIVATE ${CMAKE_SOURCE_DIR}/include)

# ---📦 Install Targets---------------------------------
install(TARGETS zuno
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)

install(FILES ${ZUNO_HEADERS}
  DESTINATION include/zuno
)