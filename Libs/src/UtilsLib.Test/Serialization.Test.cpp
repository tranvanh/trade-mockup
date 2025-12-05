#include "UtilsLib/Serialization.h"
#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

TRANVANH_NAMESPACE_BEGIN

namespace Detail {
    template <typename T>
    std::string to_string_via_stream(const T& value) {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }

    class MySerializable : public Serializable {
        int mValue = 0;

    public:
        MySerializable(const int value)
            : mValue(value) {}

    protected:
        std::ostream& serialize(std::ostream& os) const override {
            return os << "MySerializable(" << mValue << ")";
        }
    };

    // Expect to be expandable by std::range
    class MyContainer {
        std::vector<int> mData = { 1, 2, 3, 4, 5 };

    public:
        std::vector<int>::const_iterator begin() const { return mData.begin(); }
        std::vector<int>::const_iterator  end() const { return mData.end(); }
    };
} // namespace Detail

TEST(Serialization, PairBasic) {
    std::pair<int, int> p{ 1, 2 };
    EXPECT_EQ(Detail::to_string_via_stream(p), "{1: 2}");

    std::pair<std::string, int> p2{ "key", 42 };
    EXPECT_EQ(Detail::to_string_via_stream(p2), "{key: 42}");
}

TEST(Serialization, Range_Primitives) {
    std::vector<int> v{ 1, 2, 3 };
    EXPECT_EQ(Detail::to_string_via_stream(v), "[1, 2, 3]");

    std::vector<int> empty;
    EXPECT_EQ(Detail::to_string_via_stream(empty), "[]");
}

TEST(Serialization, Range_OfPairs) {
    std::vector<std::pair<int, int>> vp{ { 1, 2 }, { 3, 4 } };
    EXPECT_EQ(Detail::to_string_via_stream(vp), "[{1: 2}, {3: 4}]");
}

TEST(Serialization, NestedRanges) {
    std::vector<std::vector<int>> vv{ { 1, 2 }, { 3 } };
    EXPECT_EQ(Detail::to_string_via_stream(vv), "[[1, 2], [3]]");
}

TEST(Serialization, StringLikeNotAsRange) {
    std::string s = "abc";
    EXPECT_EQ(Detail::to_string_via_stream(s), "abc");

    std::string_view sv = "xyz";
    EXPECT_EQ(Detail::to_string_via_stream(sv), "xyz");

    const char* cstr = "hello";
    EXPECT_EQ(Detail::to_string_via_stream(cstr), "hello");
}

TEST(Serialization, SerializeCustomContainer) {
    Detail::MyContainer container;
    EXPECT_EQ(Detail::to_string_via_stream(container), "[1, 2, 3, 4, 5]");
}

TEST(Serialization, SerializableInterface) {
    Detail::MySerializable obj(7);
    EXPECT_EQ(Detail::to_string_via_stream(obj), "MySerializable(7)");
}

TRANVANH_NAMESPACE_END
