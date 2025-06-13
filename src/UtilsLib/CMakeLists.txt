cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)
FetchContent_Declare(
  Boost
  URL https://github.com/boostorg/boost/releases/download/boost-1.88.0/boost-1.88.0-b2-nodocs.tar.gz
)
FetchContent_MakeAvailable(Boost)

set(BOOST_INCLUDE_LIBRARIES asio)
set(BOOST_ENABLE_CMAKE ON)

# Create a lib
file(GLOB SOURCES *.cpp *.h)
# add_definitions(-include Common.h)
add_library(UtilsLib STATIC ${SOURCES})

target_include_directories(UtilsLib PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(UtilsLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
