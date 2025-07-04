set(ZUNO_ENABLE_TESTS OFF CACHE BOOL "Enable tests")
set(ZUNO_BUILD_EXAMPLES ON CACHE BOOL "Build examples")


if(ZUNO_ENABLE_TESTS)
  add_compile_definitions(ZUNO_ENABLE_TESTS)
endif()