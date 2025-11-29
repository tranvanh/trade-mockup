#pragma once
#include <iostream>
#include <memory>
#include <utility>

#define ASSERT(Expr, Msg) IMPL_Assert(#Expr, Expr, __FILE__, __LINE__, Msg)

[[maybe_unused]] static void IMPL_Assert([[maybe_unused]] const char* expr_str,[[maybe_unused]] bool expr,[[maybe_unused]] const char* file,[[maybe_unused]] int line,[[maybe_unused]] const char* msg) {
    #if !defined(RELEASE_BUILD)
    if (!expr) {
        std::cerr << "Assert failed:\t" << msg << "\n"
                  << "Expected:\t" << expr_str << "\n"
                  << "Source:\t\t" << file << ", line " << line << "\n";
        abort();
    }
    #endif
}

#define TRANVANH_NAMESPACE_BEGIN namespace tranvanh {
#define TRANVANH_NAMESPACE_END }
