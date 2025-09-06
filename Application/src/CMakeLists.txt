find_package(UtilsLib)
find_package(TradeLib)

file(GLOB SOURCES *.cpp *.h)
add_executable(app ${SOURCES})
target_compile_options(app PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_link_libraries(app PRIVATE UtilsLib TradeLib)