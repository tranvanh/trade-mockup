cmake_minimum_required(VERSION 3.22)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.h)
add_library(TradeGUI STATIC ${SOURCES})
target_compile_options(TradeGUI PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_include_directories(TradeGUI PUBLIC include)
target_link_libraries(TradeGUI PUBLIC TradeCore component dom screen)
