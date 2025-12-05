#pragma once
#include "TradeLib/Order.h"
#include "UtilsLib/Serialization.h"
#include "UtilsLib/TimePointUtils.h"

TRANVANH_NAMESPACE_BEGIN

struct Trade : public Serializable {
    int         sellerId;
    int       buyerId;
    timepoint_t tradeTime;
    int         volume;

    Trade(const int sellerId, const int buyerId, const timepoint_t time, const int volume)
        : sellerId(sellerId)
        , buyerId(buyerId)
        , tradeTime(time)
        , volume(volume) {}
    virtual std::ostream& serialize(std::ostream& os) const override;
    static bool           compareLowerPrice(const Trade& t1, const Trade& t2);
};

TRANVANH_NAMESPACE_END
