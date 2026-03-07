#pragma once
#include "Toybox/Serialization.h"

namespace TradeCore {
    struct Trade : public toybox::Serializable {
        int         sellerId;
        int         buyerId;
        int         price;
        int         volume;
        timepoint_t tradeTime;

        Trade(const int sellerId, const int buyerId, const int price, const int volume)
            : sellerId(sellerId)
            , buyerId(buyerId)
            , price(price)
            , volume(volume)
            , tradeTime(std::chrono::system_clock::now()) {}
        virtual std::ostream& serialize(std::ostream& os) const override;
        static bool           compareLowerPrice(const Trade& t1, const Trade& t2);
    };
}
