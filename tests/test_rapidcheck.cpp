#include <gtest/gtest.h>
#include <rapidcheck.h>
#include <rapidcheck/gtest.h>

#include "sub/sub.hpp"

RC_GTEST_PROP(AddProp, Commutative, (int a, int b)) {
    RC_ASSERT(Add(a, b) == Add(b, a));
}

RC_GTEST_PROP(AddProp, IdentityWithZero, (int x)) {
    RC_ASSERT(Add(x, 0) == x);
}

RC_GTEST_PROP(AddProp, SumOfPositivesIsPositive, ()) {
    const auto a = *rc::gen::positive<int>();
    const auto b = *rc::gen::positive<int>();
    RC_ASSERT(Add(a, b) > 0);
}
