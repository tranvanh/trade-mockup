#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <optional>
#include "UtilsLib/FlatMap.h"

TRANVANH_NAMESPACE_BEGIN

TEST(FlatMap, EmptyOnConstruct) {
    FlatMap<int, int> m;
    EXPECT_TRUE(m.empty());
    EXPECT_EQ(m.size(), 0u);
    EXPECT_EQ(m.begin(), m.end());
    EXPECT_FALSE(m.contains(1));
    EXPECT_FALSE(m.get(1).has_value());
}

TEST(FlatMap, InsertAndFind) {
    FlatMap<int, int> m;
    EXPECT_TRUE(m.insert({2, 20}));
    EXPECT_TRUE(m.insert({1, 10}));
    EXPECT_TRUE(m.insert({3, 30}));
    EXPECT_EQ(m.size(), 3u);

    // Should be sorted by key
    std::vector<int> keys;
    for (const auto& kv : m) keys.push_back(kv.first);
    EXPECT_EQ((std::vector<int>{1,2,3}), keys);

    // find/get/contains
    auto it = m.find(2);
    ASSERT_NE(it, m.end());
    EXPECT_EQ(it->first, 2);
    EXPECT_EQ(it->second, 20);
    EXPECT_TRUE(m.contains(3));
    EXPECT_EQ(m.get(1), std::optional<int>(10));
    EXPECT_EQ(m.find(42), m.end());
    EXPECT_FALSE(m.get(42).has_value());
}

TEST(FlatMap, DuplicateInsertRejected) {
    FlatMap<int, int> m;
    EXPECT_TRUE(m.insert({5, 50}));
    EXPECT_FALSE(m.insert({5, 500}));
    EXPECT_EQ(m.size(), 1u);
    EXPECT_EQ(m.get(5), std::optional<int>(50));
}

TEST(FlatMap, EraseExistingAndMissing) {
    FlatMap<int, int> m;
    for (int i = 0; i < 5; ++i) EXPECT_TRUE(m.insert({i, i*10}));
    EXPECT_TRUE(m.erase(2));
    EXPECT_FALSE(m.contains(2));
    EXPECT_EQ(m.size(), 4u);
    EXPECT_FALSE(m.erase(42));
    // Remaining order
    std::vector<int> keys;
    for (const auto& kv : m) keys.push_back(kv.first);
    EXPECT_EQ((std::vector<int>{0,1,3,4}), keys);
}

TEST(FlatMap, ClearResetsState) {
    FlatMap<int, int> m;
    EXPECT_TRUE(m.insert({1, 1}));
    EXPECT_TRUE(m.insert({2, 2}));
    m.clear();
    EXPECT_TRUE(m.empty());
    EXPECT_EQ(m.size(), 0u);
    EXPECT_EQ(m.find(1), m.end());
    EXPECT_FALSE(m.contains(2));
}

TEST(FlatMap, InsertMoveOverload) {
    FlatMap<std::string, std::string> m;
    std::pair<std::string, std::string> p{"key", "value"};
    EXPECT_TRUE(m.insert(std::move(p)));
    EXPECT_EQ(m.size(), 1u);
    // Verify content without using find/get (TValue is std::string)
    ASSERT_NE(m.begin(), m.end());
    EXPECT_EQ(m.begin()->first, std::string("key"));
    EXPECT_EQ(m.begin()->second, std::string("value"));
}

TEST(FlatMap, CustomComparatorDescending) {
    FlatMap<int, int> m{std::function<bool(const int&, const int&)>([](const int& a, const int& b){ return a > b; })};
    EXPECT_TRUE(m.insert({1,10}));
    EXPECT_TRUE(m.insert({3,30}));
    EXPECT_TRUE(m.insert({2,20}));
    // Order should be descending by key: 3,2,1
    std::vector<int> keys;
    for (const auto& kv : m) keys.push_back(kv.first);
    EXPECT_EQ((std::vector<int>{3,2,1}), keys);
    // Search should still work with custom comparator
    EXPECT_TRUE(m.contains(2));
    EXPECT_EQ(m.get(2), std::optional<int>(20));
    EXPECT_EQ(m.find(42), m.end());
}

TRANVANH_NAMESPACE_END
