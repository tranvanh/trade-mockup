#pragma once
#include "Common.h"
#include <sstream>
#include <ranges>

TRANVANH_NAMESPACE_BEGIN

/// Concept for pairs
template<typename T>
concept Pair = requires {
    typename T::first_type;
    typename T::second_type;
};

/// Overload for pairs
template<Pair P>
std::ostream& operator<<(std::ostream& os, const P& p) {
    return os << "{" << p.first << ": " << p.second << "}";
}

/// Helper traits to check for string like structures
template<typename T>
struct is_string_like : std::false_type {};

/// Template specializations
/// template<> struct is_string_like<std::string> : std::true_type {};
///         ^^ no template left     ^^^^^^^^^^^^ specialization
template<> struct is_string_like<std::string> : std::true_type {};
template<> struct is_string_like<std::string_view> : std::true_type {};
template<> struct is_string_like<const char*> : std::true_type {};
template<> struct is_string_like<char*> : std::true_type {};
template<size_t N> struct is_string_like<char[N]> : std::true_type {};
template<size_t N> struct is_string_like<const char[N]> : std::true_type {};

/// Concept for range expandable containers
///
/// Custom containers must have T* begin() and T* end() implemented
/// Don't match strings, they have their own serialization
template<typename T>
concept SerializableRange = std::ranges::range<T>
    && !is_string_like<std::remove_cvref_t<T>>::value;

/// Overload for range expandable containers
template<SerializableRange SR>
std::ostream& operator<<(std::ostream& os, const SR& range) {
    os << "[";
    bool first = true;
    for (const auto& elem : range) {
        if (!first) os << ", ";
        first = false;
        os << elem;
    }
    os << "]";
    return os;
}

/// Interface used for Seriazable objects
class Serializable {
protected:
    virtual ~Serializable() = default;
    virtual std::ostream& serialize(std::ostream& os) const = 0;

public:
    friend std::ostream& operator<<(std::ostream& os, const Serializable& trade);
};

TRANVANH_NAMESPACE_END
