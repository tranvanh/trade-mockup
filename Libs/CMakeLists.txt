cmake_minimum_required(VERSION 3.22)
project(Libs)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_compile_options(-Wall -Wextra -Wpedantic -Werror)
include(FetchContent)

if (NOT (CMAKE_GENERATOR STREQUAL "Ninja Multi-Config"))
    message(FATAL_ERROR
        "This project must be built with Ninja Multi-Config.\n"
        "Run cmake with: -G \"Ninja Multi-Config\""
    )
endif()

# Add Assert as a custom configuration
list(APPEND CMAKE_CONFIGURATION_TYPES "Assert")
list(REMOVE_DUPLICATES CMAKE_CONFIGURATION_TYPES)
set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "" FORCE)

# Set compiler flags for Assert configuration
set(CMAKE_CXX_FLAGS_ASSERT "${CMAKE_CXX_FLAGS_DEBUG}" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_ASSERT "${CMAKE_C_FLAGS_DEBUG}" CACHE STRING "" FORCE)

# Option to build tests
option(BUILD_TESTS "Build tests" OFF)
enable_testing()
if(BUILD_TESTS)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
        GIT_SHALLOW TRUE
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

# Option to build tests
option(BUILD_BENCHMARK "Build benchmark" OFF)
enable_testing()
if(BUILD_BENCHMARK)
    FetchContent_Declare(
        googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.9.4
        GIT_SHALLOW TRUE
    )
    # Configure benchmark options before making it available
    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Disable benchmark tests" FORCE)
    set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "Disable gtest tests in benchmark" FORCE)
    FetchContent_MakeAvailable(googlebenchmark)
endif()

add_subdirectory(src)