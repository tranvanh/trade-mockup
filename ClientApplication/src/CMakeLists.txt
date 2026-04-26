file(GLOB SOURCES *.cpp *.h)
add_executable(ClientApplication ${SOURCES})
target_compile_options(ClientApplication PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_link_libraries(ClientApplication PRIVATE TradeCore TradeGUI nlohmann_json::nlohmann_json Boost::headers)
