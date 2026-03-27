file(GLOB SOURCES *.cpp)
add_executable(TradeCore.Sandbox ${SOURCES})
target_link_libraries(TradeCore.Sandbox PRIVATE TradeCore)
