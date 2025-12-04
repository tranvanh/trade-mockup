#pragma once
#include "TradeLib/Order.h"
#include "UtilsLib/TimePointUtils.h"
#include "UtilsLib/Serialization.h"

TRANVANH_NAMESPACE_BEGIN

struct Trade : public Serializable{
    Order       seller;
    Order       buyer;
    timepoint_t tradeTime;
    int         volume;

    Trade(const Order& seller, const Order& buyer, const timepoint_t time, const int volume) : seller(seller), buyer(buyer), tradeTime(time), volume(volume) {}
    virtual std::ostream& serialize(std::ostream& os) const override;
    static bool          compareLowerPrice(const Trade& t1, const Trade& t2);
};

TRANVANH_NAMESPACE_END
