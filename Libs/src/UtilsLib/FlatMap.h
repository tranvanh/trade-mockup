#include "UtilsLib/Common.h"
#include "UtilsLib/Serialization.h"
#include <algorithm>
#include <functional>
#include <optional>
#include <vector>

TRANVANH_NAMESPACE_BEGIN

template <typename T>
concept FlatMapKey = std::totally_ordered<T> && std::default_initializable<T> && std::copyable<T> &&
                     std::movable<T> && !std::is_reference_v<T> && !std::is_const_v<T>;
template <typename T>
concept FlatMapValue = std::destructible<T> && !std::is_reference_v<T>;

template <FlatMapKey TKey, FlatMapValue TValue, size_t TInitSize = 10>
requires(TInitSize > 0)

class FlatMap {
    using Item          = std::pair<TKey, TValue>;
    using Iterator      = std::vector<Item>::iterator;
    using ConstIterator = std::vector<Item>::const_iterator;

    std::vector<Item>                             mData;
    std::function<bool(const Item&, const Item&)> mComp;

    size_t mFilledSize = 0;

public:
    FlatMap()
        : mComp([](const Item& a, const Item& b) {
            return a.first < b.first;
        }) {
        mData.reserve(TInitSize);
    }
    FlatMap(std::function<bool(const TKey&, const TKey&)>&& comp)
        : mComp([comp = std::move(comp)](const Item& a, const Item& b) {
            return comp(a.first, b.first);
        }) {
        mData.reserve(TInitSize);
    }

    bool erase(const TKey& key) {
        auto pos = find(key);
        if (pos == end()) {
            return false;
        }
        mData.erase(pos);
        --mFilledSize;
        return true;
    }

    bool insert(const Item& item) {
        auto range = std::equal_range(mData.begin(), mData.begin() + mFilledSize, item, mComp);
        if (range.first != range.second) {
            return false;
        }
        mData.insert(range.first, item);
        ++mFilledSize;
        return true;
    }

    bool insert(Item&& item) {
        auto range = std::equal_range(mData.begin(), mData.begin() + mFilledSize, item, mComp);
        if (range.first != range.second) {
            return false;
        }
        mData.insert(range.first, std::move(item));
        ++mFilledSize;
        return true;
    }

    Iterator find(const TKey& key) {
        auto range = std::equal_range(mData.begin(), mData.end(), std::make_pair(key, 0), mComp);
        return range.first == range.second ? end() : range.first;
    }

    ConstIterator find(const TKey& key) const {
        auto range = std::equal_range(mData.cbegin(), mData.cend(), std::make_pair(key, 0), mComp);
        return range.first == range.second ? cend() : range.first;
    }

    std::optional<TValue> get(const TKey& key) const {
        auto pos = find(key);
        return pos != end() ? std::optional<TValue>(pos->second) : std::nullopt;
    }

    bool contains(const TKey& key) const { return find(key) != end(); }

    bool   empty() const { return mData.empty(); }
    size_t size() const { return mData.size(); }
    void   clear() {
        mData.clear();
        mFilledSize = 0;
        mData.reserve(TInitSize);
    }
    Iterator      begin() { return mData.begin(); }
    ConstIterator begin() const { return mData.begin(); }
    ConstIterator cbegin() const noexcept { return mData.cbegin(); }

    Iterator      end() { return mData.end(); }
    ConstIterator end() const { return mData.end(); }
    ConstIterator cend() const noexcept { return mData.cend(); }
};

TRANVANH_NAMESPACE_END
