#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "sub/sub.hpp"

TEST_CASE("Add function - integer", "[add]") {
    REQUIRE(Add(2, 3) == 5);
    REQUIRE(Add(0, 3) == 3);
    REQUIRE(Add(-1, 1) == 0);
}

TEST_CASE("Add function - double", "[add]") {
    REQUIRE(Add(2.0, 3.0) == Catch::Approx(5.0));
    REQUIRE(Add(-1.5, -1.5) == Catch::Approx(-3.0));
}
