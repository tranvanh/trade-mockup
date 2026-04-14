#pragma once
#include <cstddef>
#include <deque>
#include <mutex>
#include <vector>

namespace TradeCommon {

    template<typename T>
    class BoundedHistory {
        std::size_t         mCapacity = 0;
        mutable std::mutex  mMutex;
        std::deque<T>       mEntries;

    public:
        explicit BoundedHistory(const std::size_t capacity)
            : mCapacity(capacity) {}

        void push(T entry) {
            std::lock_guard<std::mutex> lock(mMutex);
            if (mCapacity == 0) {
                return;
            }
            if (mEntries.size() == mCapacity) {
                mEntries.pop_front();
            }
            mEntries.push_back(std::move(entry));
        }

        void clear() {
            std::lock_guard<std::mutex> lock(mMutex);
            mEntries.clear();
        }

        [[nodiscard]] std::vector<T> snapshot() const {
            std::lock_guard<std::mutex> lock(mMutex);
            return {mEntries.begin(), mEntries.end()};
        }

        [[nodiscard]] std::size_t size() const {
            std::lock_guard<std::mutex> lock(mMutex);
            return mEntries.size();
        }
    };
}
