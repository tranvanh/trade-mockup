#pragma once
#include "CallbackOwner.h"

struct Trade;
class StockMarket;

class DumbInvestor : CallbackOwner {
    StockMarket& mStockMarket;

public:
    DumbInvestor(StockMarket& stockMarket);

private:
    void analyzeAndDisrupt(const Trade& trade);
};