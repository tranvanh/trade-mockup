#include "TradeLib/StockMarket.h"
#include "TradeLib/Trade.h"
#include "TradeLib/Order.h"
#include <gtest/gtest.h>
#include <chrono>
#include <sstream>

TRANVANH_NAMESPACE_BEGIN

TEST(Trade, CompareLowerPriceByTimepoint) {
    // Despite the name, current implementation compares tradeTime
    Order buyer{ .clientId = 1, .timeStamp = std::chrono::system_clock::now(), .type = OrderType::BUY, .price = 100, .volume = 1 };
    Order seller{ .clientId = 2, .timeStamp = std::chrono::system_clock::now(), .type = OrderType::SELL, .price = 100, .volume = 1 };

    auto t0 = std::chrono::system_clock::now();
    auto t1 = t0 + std::chrono::seconds(1);

    Trade early{ .seller = seller, .buyer = buyer, .tradeTime = t0, .volume = 1 };
    Trade later{ .seller = seller, .buyer = buyer, .tradeTime = t1, .volume = 1 };

    EXPECT_TRUE(Trade::compareLowerPrice(early, later));
    EXPECT_FALSE(Trade::compareLowerPrice(later, early));
}

TEST(Trade, OstreamOutputsKeyFields) {
    Order buyer{ .clientId = 123, .timeStamp = std::chrono::system_clock::now(), .type = OrderType::BUY, .price = 150, .volume = 3 };
    Order seller{ .clientId = 456, .timeStamp = std::chrono::system_clock::now(), .type = OrderType::SELL, .price = 150, .volume = 3 };
    Trade trade{ .seller = seller, .buyer = buyer, .tradeTime = std::chrono::system_clock::now(), .volume = 3 };

    std::ostringstream oss;
    oss << trade;
    const auto s = oss.str();

    // Expect the formatted output to contain buyer/seller IDs and volume
    EXPECT_NE(s.find("buyer="), std::string::npos);
    EXPECT_NE(s.find("seller="), std::string::npos);
    EXPECT_NE(s.find("price="), std::string::npos);
    EXPECT_NE(s.find("volume="), std::string::npos);
}

TRANVANH_NAMESPACE_END
