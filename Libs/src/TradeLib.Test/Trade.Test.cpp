#include "TradeLib/Trade.h"
#include "TradeLib/Order.h"
#include "TradeLib/StockMarket.h"
#include <chrono>
#include <gtest/gtest.h>
#include <sstream>

TRANVANH_NAMESPACE_BEGIN

TEST(Trade, CompareLowerPriceByTimepoint) {
    // Despite the name, current implementation compares tradeTime
    Order buyer{ 1, std::chrono::system_clock::now(), OrderType::BUY, 100, 1 };
    Order seller{ 2, std::chrono::system_clock::now(), OrderType::SELL, 100, 1 };

    auto t0 = std::chrono::system_clock::now();
    auto t1 = t0 + std::chrono::seconds(1);

    Trade early{ seller, buyer, t0, 1 };
    Trade later{ seller, buyer, t1, 1 };

    EXPECT_TRUE(Trade::compareLowerPrice(early, later));
    EXPECT_FALSE(Trade::compareLowerPrice(later, early));
}

TEST(Trade, OstreamOutputsKeyFields) {
    Order buyer{ 123, std::chrono::system_clock::now(), OrderType::BUY, 150, 3 };
    Order seller{ 456, std::chrono::system_clock::now(), OrderType::SELL, 150, 3 };
    Trade trade{ seller, buyer, std::chrono::system_clock::now(), 3 };

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
