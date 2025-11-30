file(GLOB SOURCES *.cpp)
add_executable(TradeLib.Benchmark ${SOURCES})
target_link_libraries(TradeLib.Benchmark PRIVATE
    TradeLib
    UtilsLib
    benchmark::benchmark_main
)
