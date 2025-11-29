cmake_minimum_required(VERSION 3.22)
project(TradeServer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

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

add_subdirectory(src)

# Set package metadata
set(CPACK_PACKAGE_NAME "ServerApp")
set(CPACK_PACKAGE_VENDOR "tranvanh")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Server Application for Trade Mockup. Receives and processes trade orders.")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")

# Set package generators
# For Linux: DEB, RPM, TGZ
# For Windows: NSIS, ZIP
# For macOS: DragNDrop, TGZ
set(CPACK_GENERATOR "TGZ")
# Include CPack module
include(CPack)