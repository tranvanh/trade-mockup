file(GLOB SOURCES *.cpp *.h)
add_library(TradeCommon STATIC ${SOURCES})
target_include_directories(TradeCommon PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(
    TradeCommon
    PUBLIC TradeCore nlohmann_json::nlohmann_json cxxopts Boost::headers
    PRIVATE Threads::Threads)
target_compile_options(TradeCommon PRIVATE -Wall -Wextra -Wpedantic -Werror)
