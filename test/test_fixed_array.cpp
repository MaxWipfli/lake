/*
 * SPDX-FileCopyrightText: 2023 Max Wipfli <mail@maxwipfli.ch>
 * SPDX-LicenseIdentifier: MIT
 */

#include "utils.hpp"
#include <gtest/gtest.h>
#include <lake/fixed_array.hpp>

TEST(FixedArray, Empty)
{
    lake::fixed_array<u16> fa;
    EXPECT_EQ(fa.size(), 0);
    EXPECT_TRUE(fa.empty());
    EXPECT_EQ(fa.begin(), fa.end());
    EXPECT_EQ(fa.span(), lake::span<u16>());

    EXPECT_DEATH((void)fa[0], "");
    EXPECT_DEATH((void)fa.front(), "");
    EXPECT_DEATH((void)fa.back(), "");
}

TEST(FixedArray, Basic)
{
    auto il = { 1, 2, 3 };
    lake::fixed_array<int> fa = il;
    EXPECT_EQ(fa.size(), 3);
    EXPECT_EQ(fa[0], 1);
    EXPECT_EQ(fa[1], 2);
    EXPECT_EQ(fa[2], 3);

    fa[0] = 42;
    EXPECT_EQ(fa[0], 42);
    EXPECT_EQ(*il.begin(), 1);
}

TEST(FixedArray, DestructionCounts)
{
    int destruction_count = 0;
    {
        lake::fixed_array<destruction_counter> fa = { destruction_counter(&destruction_count) };
        // The count is already 1 here as the destruction_counter in the initalizer list has already been destroyed.
        EXPECT_EQ(destruction_count, 1);
    }
    EXPECT_EQ(destruction_count, 2);
}

// TODO: Add some more tests, similar to test_vector.
