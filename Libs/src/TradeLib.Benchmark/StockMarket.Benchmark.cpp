#include "TradeLib/Market.h"
#include <benchmark/benchmark.h>

TRANVANH_NAMESPACE_BEGIN

// Benchmark constructing a StockMarket instance
static void BM_StockMarket_Construct(benchmark::State& state) {
    for (auto _ : state) {
        Market market;
        benchmark::DoNotOptimize(market.isActive());
    }
}
BENCHMARK(BM_StockMarket_Construct);

// Benchmark adding a simple on-trade observer
static void BM_StockMarket_AddObserver(benchmark::State& state) {
    for (auto _ : state) {
        Market market;
        auto        lifetime = market.addOnTradeObserver([](const Trade&) {});
        benchmark::DoNotOptimize(lifetime);
    }
}
BENCHMARK(BM_StockMarket_AddObserver);

// Benchmark registering a single order
static void BM_StockMarket_RegisterOrder(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        auto  market = std::make_unique<Market>();
        Order order{
            .clientId  = 42,
            .timeStamp = std::chrono::system_clock::now(),
            .type      = OrderType::BUY,
            .price     = 100,
            .volume    = 10,
        };
        state.ResumeTiming();
        market->registerOrder(order);
        benchmark::ClobberMemory();
        state.PauseTiming();
        market.reset();
        state.ResumeTiming();
    }
}
BENCHMARK(BM_StockMarket_RegisterOrder);

TRANVANH_NAMESPACE_END
