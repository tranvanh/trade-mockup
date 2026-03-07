include(GoogleTest)
file(GLOB SOURCES *.cpp)
add_executable(Toybox.Test ${SOURCES})
target_link_libraries(Toybox.Test PRIVATE
    Toybox
    GTest::gtest_main
)
gtest_discover_tests(Toybox.Test)
