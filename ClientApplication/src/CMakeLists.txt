set(CMAKE_PREFIX_PATH "/home/tranvanh/coding/trade-mockup/includeLibs/")

find_package(UtilsLib)
find_package(TradeLib)

file(GLOB SOURCES *.cpp *.h)
add_executable(clientApp ${SOURCES})

target_compile_options(clientApp PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_link_libraries(clientApp PRIVATE UtilsLib TradeLib)
