cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Create a lib
file(GLOB SOURCES *.cpp *.h)
# add_definitions(-include Common.h)
add_library(UtilsLib STATIC ${SOURCES})

# Expose headers to other targets
target_include_directories(UtilsLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})