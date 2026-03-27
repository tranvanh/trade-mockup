file(GLOB SOURCES *.cpp)
add_executable(TradeCore.Test ${SOURCES})
target_link_libraries(TradeCore.Test PRIVATE TradeCore GTest::gtest_main)
gtest_discover_tests(TradeCore.Test)
