file(GLOB SOURCES *.cpp)
add_executable(TradeCore.Benchmark ${SOURCES})
target_link_libraries(TradeCore.Benchmark PRIVATE TradeCore benchmark::benchmark_main)
