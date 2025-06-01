cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Create a library
file(GLOB SOURCES *.cpp *.h)
add_library(TradeLib SHARED ${SOURCES})
add_definitions(-include Common.h)

# Link the static library 'UtilsLib'
target_link_libraries(TradeLib PUBLIC UtilsLib)

# Expose headers to other targets
target_include_directories(TradeLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})