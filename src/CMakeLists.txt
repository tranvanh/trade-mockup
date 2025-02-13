cmake_minimum_required(VERSION 3.10)

project(Trade)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB src *.cpp)

include_directories( ${PROJECT_SOURCE_DIR})
add_executable(Trade main.cpp)
add_definitions(-include Common.h)
target_sources(Trade PRIVATE ${src})