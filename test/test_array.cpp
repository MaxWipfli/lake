/*
 * SPDX-FileCopyrightText: 2022-2023 Max Wipfli <mail@maxwipfli.ch>
 * SPDX-LicenseIdentifier: MIT
 */

#include <gtest/gtest.h>
#include <lake/array.hpp>

TEST(Array, Basic)
{
    lake::array<u8, 2> arr = { 1, 2 };
    EXPECT_EQ(arr.size(), 2);

    EXPECT_EQ(arr.at(0), 1);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr.at(1), 2);
    EXPECT_EQ(arr[1], 2);

    EXPECT_EQ(&arr.front(), &arr[0]);
    EXPECT_EQ(&arr.back(), &arr[1]);
}

TEST(Array, Fill)
{
    auto arr = lake::array<u8, 2>::filled(42);
    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr[0], 42);
    EXPECT_EQ(arr[1], 42);
}

TEST(Array, Span)
{
    auto arr = lake::array<u8, 4>::filled(42);
    auto span = arr.span();
    EXPECT_EQ(span.data(), arr.data());
    EXPECT_EQ(span.size(), arr.size());
}
// FIXME: Copy most tests from test_span.cpp.
