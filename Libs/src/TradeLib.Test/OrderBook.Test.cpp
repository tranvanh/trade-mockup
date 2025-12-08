#include <gtest/gtest.h>
#include <vector>
#include <utility>
#include "TradeLib/OrderBook.h"
#include "TradeLib/Order.h"
#include "TradeLib/Trade.h"

TRANVANH_NAMESPACE_BEGIN

// Helper: compare that a trade involves exactly these two client IDs (order-agnostic)
static bool tradeInvolvesClients(const Trade& t, int idA, int idB) {
    return (t.buyerId == idA && t.sellerId == idB) || (t.buyerId == idB && t.sellerId == idA);
}

TEST(OrderBook, DeterministicBuyPriorityAndFifoWithinLevel) {
    OrderBook ob;
    std::vector<Trade> trades;
    auto lifetime = ob.onTradeCallbacks.add([&](const Trade& tr){ trades.push_back(tr); });

    // Build book: buyers at different prices; ensure price-priority (highest first) and FIFO within same price
    Order b2(2, OrderType::BUY, 110, 2); // best price level, first in FIFO at 110
    Order b3(3, OrderType::BUY, 110, 2); // same price level, second in FIFO at 110
    Order b1(1, OrderType::BUY, 100, 2); // worse price level

    ob.registerOrder(b2); ob.pollOrders();
    ob.registerOrder(b3); ob.pollOrders();
    ob.registerOrder(b1); ob.pollOrders();

    // Incoming seller crosses all: total volume 5, price 90 (crosses 110 and 100)
    Order s1(10, OrderType::SELL, 90, 5);
    ob.registerOrder(s1); ob.pollOrders();

    ASSERT_EQ(trades.size(), 3u);

    // Expect to fill buyers at 110 first (b2 then b3), then the remaining with b1 at 100.
    // Matching price in current implementation is min(requester.price, resting.price) => here it's 90 for fills against 110 and 100.
    EXPECT_EQ(trades[0].volume, 2);
    EXPECT_EQ(trades[1].volume, 2);
    EXPECT_EQ(trades[2].volume, 1);

    EXPECT_EQ(trades[0].price, 90);
    EXPECT_EQ(trades[1].price, 90);
    EXPECT_EQ(trades[2].price, 90);

    // Ensure counterparties are as expected per FIFO within the best price level, then next level
    EXPECT_TRUE(tradeInvolvesClients(trades[0], 10, 2)); // seller 10 vs buyer 2 first
    EXPECT_TRUE(tradeInvolvesClients(trades[1], 10, 3)); // then buyer 3
    EXPECT_TRUE(tradeInvolvesClients(trades[2], 10, 1)); // finally buyer 1 at worse price
}

TEST(OrderBook, DeterministicSellPriorityAndFifoWithinLevel) {
    OrderBook ob;
    std::vector<Trade> trades;
    auto lifetime = ob.onTradeCallbacks.add([&](const Trade& tr){ trades.push_back(tr); });

    // Build book: sellers at different prices; ensure lowest price first and FIFO within same price
    Order s2(20, OrderType::SELL, 90, 2); // best (lowest) price level, first in FIFO at 90
    Order s3(30, OrderType::SELL, 90, 2); // same price level, second in FIFO at 90
    Order s1(10, OrderType::SELL, 100, 2); // worse price level

    ob.registerOrder(s2); ob.pollOrders();
    ob.registerOrder(s3); ob.pollOrders();
    ob.registerOrder(s1); ob.pollOrders();

    // Incoming buyer crosses all: total volume 5, price 110
    Order b1(1, OrderType::BUY, 110, 5);
    ob.registerOrder(b1); ob.pollOrders();

    ASSERT_EQ(trades.size(), 3u);

    // Expect fills against lowest price sells first (s2 then s3), then remaining with s1 at 100
    EXPECT_EQ(trades[0].volume, 2);
    EXPECT_EQ(trades[1].volume, 2);
    EXPECT_EQ(trades[2].volume, 1);

    // Price is min(requester.price, resting.price): 90 for first two, then 100
    EXPECT_EQ(trades[0].price, 90);
    EXPECT_EQ(trades[1].price, 90);
    EXPECT_EQ(trades[2].price, 100);

    EXPECT_TRUE(tradeInvolvesClients(trades[0], 1, 20));
    EXPECT_TRUE(tradeInvolvesClients(trades[1], 1, 30));
    EXPECT_TRUE(tradeInvolvesClients(trades[2], 1, 10));
}

TEST(OrderBook, PartialFillLeavesRestingVolume) {
    OrderBook ob;
    std::vector<Trade> trades;
    auto lifetime = ob.onTradeCallbacks.add([&](const Trade& tr){ trades.push_back(tr); });

    // Resting buy larger than incoming sell
    Order b1(1, OrderType::BUY, 100, 5);
    ob.registerOrder(b1); ob.pollOrders();

    Order s1(2, OrderType::SELL, 90, 3);
    ob.registerOrder(s1); ob.pollOrders();

    ASSERT_EQ(trades.size(), 1u);
    EXPECT_EQ(trades[0].volume, 3);
    EXPECT_EQ(trades[0].price, 90);
    EXPECT_TRUE(tradeInvolvesClients(trades[0], 1, 2));

    // Add another seller to consume remaining 2
    Order s2(3, OrderType::SELL, 95, 2);
    ob.registerOrder(s2); ob.pollOrders();

    ASSERT_EQ(trades.size(), 2u);
    EXPECT_EQ(trades[1].volume, 2);
    EXPECT_EQ(trades[1].price, 95);
    EXPECT_TRUE(tradeInvolvesClients(trades[1], 1, 3));
}

TEST(OrderBook, NonCrossingOrdersProduceNoTrades) {
    OrderBook ob;
    std::vector<Trade> trades;
    auto lifetime = ob.onTradeCallbacks.add([&](const Trade& tr){ trades.push_back(tr); });

    // Buyers below sellers: no crossing should occur
    Order b1(1, OrderType::BUY, 90, 2);
    Order s1(2, OrderType::SELL, 110, 2);
    ob.registerOrder(b1); ob.pollOrders();
    ob.registerOrder(s1); ob.pollOrders();

    EXPECT_TRUE(trades.empty());

    // Add more non-crossing orders interleaved
    Order b2(3, OrderType::BUY, 95, 1);
    Order s2(4, OrderType::SELL, 120, 1);
    ob.registerOrder(s2); ob.pollOrders();
    ob.registerOrder(b2); ob.pollOrders();

    EXPECT_TRUE(trades.empty());
}

TEST(OrderBook, InterleavedArrivalDeterministicSequenceOnSells) {
    OrderBook ob;
    std::vector<Trade> trades;
    auto lifetime = ob.onTradeCallbacks.add([&](const Trade& tr){ trades.push_back(tr); });

    // Build sellers with two orders at same price (FIFO) and one at worse price
    Order s1(10, OrderType::SELL, 110, 2); // first at 110
    Order s2(20, OrderType::SELL, 110, 3); // second at 110
    Order s3(30, OrderType::SELL, 120, 2); // next level

    // Interleave with some buyers that don't cross yet
    Order bnc1(101, OrderType::BUY, 90, 5);
    ob.registerOrder(bnc1); ob.pollOrders();
    ob.registerOrder(s1); ob.pollOrders();
    ob.registerOrder(s2); ob.pollOrders();
    ob.registerOrder(s3); ob.pollOrders();

    // Now send a large buyer that sweeps levels deterministically: 110 FIFO first, then 120
    Order b1(1, OrderType::BUY, 130, 6);
    ob.registerOrder(b1); ob.pollOrders();

    ASSERT_EQ(trades.size(), 3u);
    // Expect fills in FIFO at 110 (2, then 3), then remaining 1 at 120
    EXPECT_EQ(trades[0].volume, 2);
    EXPECT_EQ(trades[1].volume, 3);
    EXPECT_EQ(trades[2].volume, 1);

    EXPECT_EQ(trades[0].price, 110);
    EXPECT_EQ(trades[1].price, 110);
    EXPECT_EQ(trades[2].price, 120);

    EXPECT_TRUE(tradeInvolvesClients(trades[0], 1, 10));
    EXPECT_TRUE(tradeInvolvesClients(trades[1], 1, 20));
    EXPECT_TRUE(tradeInvolvesClients(trades[2], 1, 30));
}

TEST(OrderBook, SequentialPartialFillsPreserveFIFO) {
    OrderBook ob;
    std::vector<Trade> trades;
    auto lifetime = ob.onTradeCallbacks.add([&](const Trade& tr){ trades.push_back(tr); });

    // Two sellers at same price, s1 then s2
    Order s1(10, OrderType::SELL, 90, 3);
    Order s2(20, OrderType::SELL, 90, 3);
    ob.registerOrder(s1); ob.pollOrders();
    ob.registerOrder(s2); ob.pollOrders();

    // First buyer partially fills s1 only
    Order b1(1, OrderType::BUY, 100, 2);
    ob.registerOrder(b1); ob.pollOrders();

    ASSERT_EQ(trades.size(), 1u);
    EXPECT_EQ(trades[0].volume, 2);
    EXPECT_EQ(trades[0].price, 90);
    EXPECT_TRUE(tradeInvolvesClients(trades[0], 1, 10));

    // Next buyer should continue with remaining of s1 (FIFO preserved), not s2
    Order b2(2, OrderType::BUY, 100, 1);
    ob.registerOrder(b2); ob.pollOrders();

    ASSERT_EQ(trades.size(), 2u);
    EXPECT_EQ(trades[1].volume, 1);
    EXPECT_EQ(trades[1].price, 90);
    EXPECT_TRUE(tradeInvolvesClients(trades[1], 2, 10)); // still against s1
}

TEST(OrderBook, SweepRemovesLevelsNoPhantomTrades) {
    OrderBook ob;
    std::vector<Trade> trades;
    auto lifetime = ob.onTradeCallbacks.add([&](const Trade& tr){ trades.push_back(tr); });

    // Prepare two seller levels to be fully consumed
    Order s1(10, OrderType::SELL, 90, 1);
    Order s2(20, OrderType::SELL, 100, 1);
    ob.registerOrder(s1); ob.pollOrders();
    ob.registerOrder(s2); ob.pollOrders();

    // Buyer sweeps both
    Order b1(1, OrderType::BUY, 150, 2);
    ob.registerOrder(b1); ob.pollOrders();

    ASSERT_EQ(trades.size(), 2u);
    EXPECT_EQ(trades[0].price, 90);
    EXPECT_EQ(trades[1].price, 100);

    // After sweep, those levels should be gone; a small buyer at 95 should not trade
    Order b2(2, OrderType::BUY, 95, 1);
    ob.registerOrder(b2); ob.pollOrders();

    EXPECT_EQ(trades.size(), 2u); // no new trades
}

TRANVANH_NAMESPACE_END
