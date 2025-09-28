include(GoogleTest)
file(GLOB SOURCES *.cpp)
add_executable(UtilsLib.Test ${SOURCES})
target_link_libraries(UtilsLib.Test PRIVATE
    UtilsLib
    GTest::gtest_main
)
gtest_discover_tests(UtilsLib.Test)
