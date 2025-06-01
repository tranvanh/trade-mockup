cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB src *.cpp *.h)
add_executable(app main.cpp)
target_compile_options(app PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_link_libraries(app PRIVATE UtilsLib TradeLib)