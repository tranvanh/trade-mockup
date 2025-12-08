file(GLOB SOURCES *.cpp)
add_executable(UtilsLib.Benchmark ${SOURCES})
target_link_libraries(UtilsLib.Benchmark PRIVATE
    UtilsLib
    benchmark::benchmark_main
)
