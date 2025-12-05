add_subdirectory(TradeLib)
add_subdirectory(UtilsLib)
add_subdirectory(UtilsLib.Sandbox)
if(BUILD_TESTS)
    add_subdirectory(UtilsLib.Test)
    add_subdirectory(TradeLib.Test)
endif()

if(BUILD_BENCHMARK)
    add_subdirectory(TradeLib.Benchmark)
endif()

