#pragma once
#include "TradeLib/Order.h"
#include "UtilsLib/TimePointUtils.h"

TRANVANH_NAMESPACE_BEGIN

struct Trade {
    Order       seller;
    Order       buyer;
    timepoint_t tradeTime;
    int         volume;

    friend std::ostream& operator<<(std::ostream& os, const Trade& trade);
    static bool          compareLowerPrice(const Trade& t1, const Trade& t2);
};

TRANVANH_NAMESPACE_END
