#include <map>
#include <vector>
#include <benchmark/benchmark.h>

#include "UtilsLib/Common.h"

TRANVANH_NAMESPACE_BEGIN

// Benchmark registering a single order to a filled book
static void BM_StdVector_LinearRead(benchmark::State& state) {
    std::vector<int> testVector(state.range(0), 0);

    for (auto _ : state) {
        for (const auto& item : testVector) {
            [[maybe_unused]] int read = item;
            benchmark::DoNotOptimize(read);
        }
        benchmark::ClobberMemory();
    }
}

BENCHMARK(BM_StdVector_LinearRead)->Arg(10000);

// Benchmark registering a single order to a filled book
static void BM_StdMap_LinearReverseInsert(benchmark::State& state) {
    std::map<int, int> testMap;
    int endRange = state.range(0);
    for (auto _ : state) {
        for (int i = endRange; i >= 0; --i) {
            testMap.insert({i, 0});
        }
        benchmark::ClobberMemory();
        state.PauseTiming();
            testMap.clear();
        state.ResumeTiming();
    }
}
BENCHMARK(BM_StdMap_LinearReverseInsert)->Arg(10000);

// Benchmark registering a single order to a filled book
static void BM_StdMap_LinearRead(benchmark::State& state) {
    std::map<int, int> testMap;
    int endRange = state.range(0);
    for (int i = endRange; i >= 0; --i) {
        testMap.insert({i, 0});
    }

    for (auto _ : state) {
        for (const auto& item : testMap) {
            [[maybe_unused]] int read = item.second;
            benchmark::DoNotOptimize(read);
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_StdMap_LinearRead)->Arg(10000);

TRANVANH_NAMESPACE_END
