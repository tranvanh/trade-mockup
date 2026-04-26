cmake_minimum_required(VERSION 3.22)
project(TradeMockup)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (NOT (CMAKE_GENERATOR STREQUAL "Ninja Multi-Config"))
    message(FATAL_ERROR
            "This project must be built with Ninja Multi-Config.\n"
            "Run cmake with: -G \"Ninja Multi-Config\""
    )
endif()

list(APPEND CMAKE_CONFIGURATION_TYPES "Assert")
list(REMOVE_DUPLICATES CMAKE_CONFIGURATION_TYPES)
set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "" FORCE)

include(FetchContent)

FetchContent_Declare(
  toybox
  GIT_REPOSITORY https://github.com/tranvanh/toybox.git
  GIT_TAG v1.0
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(toybox)

FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
  cxxopts
  GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
  GIT_TAG v3.3.1
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(cxxopts)

FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
  GIT_TAG v5.0.0
  GIT_SHALLOW TRUE
)
set(FTXUI_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(FTXUI_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(FTXUI_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(ftxui)

find_package(Boost REQUIRED)

add_subdirectory(TradeCore)
add_subdirectory(TradeGUI)
add_subdirectory(ClientApplication)
add_subdirectory(ServerApplication)

option(BUILD_TESTS "Build tests" OFF)
option(BUILD_BENCHMARK "Build benchmarks" OFF)

if(BUILD_TESTS)
    FetchContent_Declare(
      googletest
      GIT_REPOSITORY https://github.com/google/googletest.git
      GIT_TAG v1.14.0
      GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(googletest)
    include(GoogleTest)
    enable_testing()
    add_subdirectory(TradeCore.Test)
endif()

if(BUILD_BENCHMARK)
    set(BENCHMARK_ENABLE_TESTING OFF)
    FetchContent_Declare(
      benchmark
      GIT_REPOSITORY https://github.com/google/benchmark.git
      GIT_TAG v1.8.3
      GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(benchmark)
    add_subdirectory(TradeCore.Benchmark)
endif()

add_subdirectory(TradeCore.Sandbox)
