#include "DumbInvestor.h"
#include "TradeLib/StockMarket.h"
#include "UtilsLib/TimePointUtils.h"
#include "TradeLib/Trade.h"
#include "UtilsLib/Logger.h"

DumbInvestor::DumbInvestor(StockMarket& stockMarket)
    : mStockMarket(stockMarket) {
    registerCallback(mStockMarket.addOnTradeObserver([this](const Trade& trade) {
        analyzeAndDisrupt(trade);
    }));
}

void DumbInvestor::analyzeAndDisrupt(const Trade& trade) {
    const int buyPrice  = trade.buyer.price;
    const int sellPrice = trade.seller.price;

    const int DISRUPTION_VALUE = 1000;

    if ((buyPrice > sellPrice + DISRUPTION_VALUE) && trade.volume < trade.seller.volume) {
        Order order;
        order.id        = 666; // the devil
        order.price     = sellPrice + DISRUPTION_VALUE;
        order.timeStamp = std::chrono::system_clock::now();
        order.type      = OrderType::BUY;
        order.volume    = 10; // Careful buyer
        mStockMarket.registerOrder(order);
        auto& logger = Logger::instance();
        logger.log(Logger::LogLevel::INFO, "Disrupted with ", order);
    }
}