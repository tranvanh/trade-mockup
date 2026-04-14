file(GLOB SOURCES *.cpp)
add_executable(TradeCommon.Test ${SOURCES})
target_link_libraries(TradeCommon.Test PRIVATE TradeCommon TradeCore GTest::gtest_main)
gtest_discover_tests(TradeCommon.Test)
