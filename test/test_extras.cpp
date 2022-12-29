/*
 * SPDX-FileCopyrightText: 2022 Max Wipfli <mail@maxwipfli.ch>
 * SPDX-LicenseIdentifier: MIT
 */

#include <gtest/gtest.h>
#include <lake/extras.hpp>

TEST(Extras, BitCeil)
{
    EXPECT_EQ(lake::bit_ceil(0u), 1u);
    EXPECT_EQ(lake::bit_ceil(1u), 1u);
    EXPECT_EQ(lake::bit_ceil(2u), 2u);
    EXPECT_EQ(lake::bit_ceil(3u), 4u);
    EXPECT_EQ(lake::bit_ceil(4u), 4u);
    EXPECT_EQ(lake::bit_ceil(5u), 8u);
    EXPECT_EQ(lake::bit_ceil(6u), 8u);
    EXPECT_EQ(lake::bit_ceil(7u), 8u);
    EXPECT_EQ(lake::bit_ceil(8u), 8u);
    EXPECT_EQ(lake::bit_ceil(9u), 16u);
}

TEST(Extras, BitFloor)
{
    EXPECT_EQ(lake::bit_floor(0u), 0u);
    EXPECT_EQ(lake::bit_floor(1u), 1u);
    EXPECT_EQ(lake::bit_floor(2u), 2u);
    EXPECT_EQ(lake::bit_floor(3u), 2u);
    EXPECT_EQ(lake::bit_floor(4u), 4u);
    EXPECT_EQ(lake::bit_floor(5u), 4u);
    EXPECT_EQ(lake::bit_floor(6u), 4u);
    EXPECT_EQ(lake::bit_floor(7u), 4u);
    EXPECT_EQ(lake::bit_floor(8u), 8u);
    EXPECT_EQ(lake::bit_floor(9u), 8u);
}
