#include "TradeLib/Market.h"
#include <benchmark/benchmark.h>

TRANVANH_NAMESPACE_BEGIN

// Benchmark adding a simple on-trade observer
static void BM_OrderBook_AddObserver(benchmark::State& state) {
    for (auto _ : state) {
        OrderBook book;
        auto   lifetime = book.onTradeCallbacks.add([](const Trade&) {});
        benchmark::DoNotOptimize(lifetime);
    }
}
BENCHMARK(BM_OrderBook_AddObserver);

// Benchmark registering a single order
static void BM_OrderBook_SingleRegisterOrder(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        auto  book = std::make_unique<OrderBook>();
        Order order(666, OrderType::BUY, 100, 10, std::chrono::system_clock::now());
        state.ResumeTiming();
        book->registerOrder(order);
        benchmark::ClobberMemory();
        state.PauseTiming();
        book.reset();
        state.ResumeTiming();
    }
}
BENCHMARK(BM_OrderBook_SingleRegisterOrder);

// Benchmark registering a single order to a filled book
static void BM_StockMarket_FilledBookRegisterOrder(benchmark::State& state) {
    int endRange = state.range(0);
    auto  book = std::make_unique<OrderBook>();
    for (int i = endRange; i >= 0; --i) {
        Order order(666, OrderType::BUY, i, 10, std::chrono::system_clock::now());
        book->registerOrder(order);
    }
    Order order(69, OrderType::BUY, endRange+1, 10, std::chrono::system_clock::now());

    for (auto _ : state) {
        book->registerOrder(order);
        benchmark::ClobberMemory();

        // erase the previous order by buing it out
        state.PauseTiming();
        Order eraser(69, OrderType::SELL, endRange+1, 10, std::chrono::system_clock::now());
        book->registerOrder(eraser);
        state.ResumeTiming();
    }
}
BENCHMARK(BM_StockMarket_FilledBookRegisterOrder)->Arg(10000);

TRANVANH_NAMESPACE_END
