file(GLOB SOURCES *.cpp *.h)
add_executable(ServerApplication ${SOURCES})
target_compile_options(ServerApplication PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_link_libraries(ServerApplication PRIVATE TradeCore TradeGUI nlohmann_json::nlohmann_json Boost::headers)
