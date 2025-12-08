#include <map>
#include <vector>
#include <benchmark/benchmark.h>

#include "UtilsLib/Common.h"
#include "UtilsLib/FlatMap.h"

TRANVANH_NAMESPACE_BEGIN

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

static void BM_StdMap_LinearInsert(benchmark::State& state) {
    std::map<int, int> testMap;
    const int endRange = state.range(0);
    for (auto _ : state) {
        for (int i = 0; i < endRange; ++i) {
            testMap.insert({i, 0});
        }
        benchmark::ClobberMemory();
        state.PauseTiming();
        testMap.clear();
        state.ResumeTiming();
    }
}
BENCHMARK(BM_StdMap_LinearInsert)->Arg(10000);

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
BENCHMARK(BM_StdMap_LinearReverseInsert)->Arg(1000);

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

static void BM_FlatMap_LinearInsert(benchmark::State& state) {
    FlatMap<int, int> testMap;
    const int endRange = state.range(0);
    for (auto _ : state) {
        for (int i = 0; i < endRange; ++i) {
            testMap.insert({i, 0});
        }
        benchmark::ClobberMemory();
        state.PauseTiming();
        testMap.clear();
        state.ResumeTiming();
    }
}
BENCHMARK(BM_FlatMap_LinearInsert)->Arg(10000);

static void BM_FlatMap_LinearReverseInsert(benchmark::State& state) {
    FlatMap<int, int> testMap;
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
BENCHMARK(BM_FlatMap_LinearReverseInsert)->Arg(1000);

static void BM_FlatMap_LinearRead(benchmark::State& state) {
    FlatMap<int, int> testMap;
    int endRange = state.range(0);
    for (int i = endRange; i >= 0; --i) {
        testMap.insert({i, 0});
    }

    for (auto _ : state) {
        for (const auto& item : testMap) {
            auto read = item;
            benchmark::DoNotOptimize(read.first);
            benchmark::DoNotOptimize(read.second);
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_FlatMap_LinearRead)->Arg(10000);

TRANVANH_NAMESPACE_END
