include(GoogleTest)
file(GLOB SOURCES *.cpp)
add_executable(TradeLib.Test ${SOURCES})
target_link_libraries(TradeLib.Test PRIVATE
    TradeLib
    UtilsLib
    GTest::gtest_main
)
gtest_discover_tests(TradeLib.Test)
