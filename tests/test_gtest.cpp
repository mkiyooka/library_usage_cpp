#include <gtest/gtest.h>

#include "sub/sub.hpp"

TEST(AddTest, Integer) {
    EXPECT_EQ(Add(2, 3), 5);
    EXPECT_EQ(Add(0, 3), 3);
    EXPECT_EQ(Add(-1, 1), 0);
}

TEST(AddTest, Double) {
    EXPECT_DOUBLE_EQ(Add(2.0, 3.0), 5.0);
    EXPECT_DOUBLE_EQ(Add(-1.5, -1.5), -3.0);
}
