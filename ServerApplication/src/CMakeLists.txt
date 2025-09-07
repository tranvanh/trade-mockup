find_package(UtilsLib)
find_package(TradeLib)

file(GLOB SOURCES *.cpp *.h)
add_executable(serverApp ${SOURCES})

target_compile_options(serverApp PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_link_libraries(serverApp PRIVATE UtilsLib TradeLib)