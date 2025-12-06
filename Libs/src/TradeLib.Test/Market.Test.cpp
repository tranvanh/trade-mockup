#include "TradeLib/Market.h"
#include "TradeLib/Order.h"
#include "TradeLib/Trade.h"
#include <chrono>
#include <gtest/gtest.h>

TRANVANH_NAMESPACE_BEGIN

TEST(Market, IsInactiveByDefault) {
    Market market;
    EXPECT_FALSE(market.isActive());
}

TEST(Market, ObserverReceivesRegisteredTrade) {
    Market market;

    int   callbackCount = 0;
    Trade* lastTrade = nullptr;
    auto  lifetime = market.addOnTradeObserver([&](const Trade& t) {
        ++callbackCount;
        EXPECT_EQ(lastTrade, nullptr);
        lastTrade = new Trade(t);
    });

    Order buyer{
        1, std::chrono::system_clock::now(), OrderType::BUY, 100, 10,
    };
    Order seller{
        2, std::chrono::system_clock::now(), OrderType::SELL, 100, 5,
    };

    Trade trade{
        seller.clientId,
        buyer.clientId,
        std::chrono::system_clock::now(),
        5,
    };

    market.registerTrade(trade);

    EXPECT_NE(lastTrade, nullptr);
    EXPECT_EQ(callbackCount, 1);
    EXPECT_EQ(lastTrade->volume, 5);
    EXPECT_EQ(lastTrade->buyerId, 1);
    EXPECT_EQ(lastTrade->sellerId, 2);
    delete lastTrade;
}

TRANVANH_NAMESPACE_END
