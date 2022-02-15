#ifndef CPP_FEATURES_H
#define CPP_FEATURES_H

#pragma once

#include "std.h"

namespace CppFeatures
{
    struct Person
    {
        int age;
        std::string name;
    };
    /**
     * NOTE 1. struct binding
     *  auto [ a , b ] = Person{ 12 , "Alex"}; // a = 12 ; b = "Alex"
     */
    enum class Operators
    {
        Add,
        Sub,
        Mut,
        Div
    };

    /**
     * NOTE 2. Compiling time if
     */
    template <Operators op>
    struct Operation
    {
        template <typename L, typename R = L>
        auto operator()(L lhs, R rhs)
        {
            if constexpr (Operators::Add == op)
                return lhs + rhs;
            else if constexpr (Operators::Sub == op)
                return lhs - rhs;
            else if constexpr (Operators::Mut == op)
                return lhs * rhs;
            else if constexpr (Operators::Div == op)
                return lhs / rhs;
        }
    };

} // namespace CppFeatures

#endif