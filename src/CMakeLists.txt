cmake_minimum_required(VERSION 3.10)

project(Trade)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# add_definitions(-include Common.h)
add_subdirectory(TradeLib)
add_subdirectory(UtilsLib)
add_subdirectory(Application)