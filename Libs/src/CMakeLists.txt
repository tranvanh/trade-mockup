add_subdirectory(TradeCore)
add_subdirectory(TradeCore.Sandbox)
add_subdirectory(Toybox)
add_subdirectory(Toybox.Sandbox)
if(BUILD_TESTS)
    add_subdirectory(Toybox.Test)
    add_subdirectory(TradeCore.Test)
endif()

if(BUILD_BENCHMARK)
    add_subdirectory(TradeCore.Benchmark)
    add_subdirectory(Toybox.Benchmark)
endif()

