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

## OpenSSL
FetchContent_Declare(
  openssl
  URL https://www.openssl.org/source/openssl-3.0.13.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP true
)

FetchContent_MakeAvailable(openssl)
find_package(OpenSSL REQUIRED)

## asio standalone
FetchContent_Declare(
  asio
  GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
  GIT_TAG        asio-1-34-2
  DOWNLOAD_EXTRACT_TIMESTAMP true
)

FetchContent_MakeAvailable(asio)

add_compile_definitions(ASIO_STANDALONE)
add_compile_definitions(ASIO_HAS_SSL)

## nlohmann::json
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
FetchContent_MakeAvailable(nlohmann_json)

# brotli compress
FetchContent_Declare(
  brotli
  GIT_REPOSITORY https://github.com/google/brotli.git
  GIT_TAG        v1.1.0
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
FetchContent_MakeAvailable(brotli)

## FMT (log formatter)
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG        11.2.0
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
FetchContent_MakeAvailable(fmt)
