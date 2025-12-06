#pragma once
#include "TradeLib/Order.h"
#include "UtilsLib/Serialization.h"

TRANVANH_NAMESPACE_BEGIN

struct Trade : public Serializable {
    int         sellerId;
    int         buyerId;
    int         price;
    int         volume;
    timepoint_t tradeTime;

    Trade(const int sellerId, const int buyerId, const int price, const int volume, const timepoint_t time)
        : sellerId(sellerId)
        , buyerId(buyerId)
        , price(price)
        , volume(volume)
        , tradeTime(time) {}
    virtual std::ostream& serialize(std::ostream& os) const override;
    static bool           compareLowerPrice(const Trade& t1, const Trade& t2);
};

TRANVANH_NAMESPACE_END
