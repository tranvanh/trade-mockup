cmake_minimum_required(VERSION 3.22)
project(TradeClient)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

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

add_subdirectory(src)