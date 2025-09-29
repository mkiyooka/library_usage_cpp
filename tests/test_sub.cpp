#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>

#include "sub/sub.hpp"

TEST_CASE("Add function tests") {
    SUBCASE("Int Add function") {
        CHECK(Add(2, 3) == 5);
        CHECK(Add(0, 3) == 3);
    }
    SUBCASE("Double Add function") {
        CHECK(Add(2.0, 3.0) == doctest::Approx(5.0));
        CHECK(Add(0.0, 0.0) == doctest::Approx(0.0));
        CHECK(Add(-1.5, -1.5) == doctest::Approx(-3.0));
    }
}
