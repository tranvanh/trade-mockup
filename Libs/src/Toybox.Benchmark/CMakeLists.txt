file(GLOB SOURCES *.cpp)
add_executable(Toybox.Benchmark ${SOURCES})
target_link_libraries(Toybox.Benchmark PRIVATE
    Toybox
    benchmark::benchmark_main
)
