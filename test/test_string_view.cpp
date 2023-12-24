/*
 * SPDX-FileCopyrightText: 2023 Max Wipfli <mail@maxwipfli.ch>
 * SPDX-LicenseIdentifier: MIT
 */

#include "utils.hpp"
#include <gtest/gtest.h>
#include <lake/string_view.hpp>

TEST(StringView, Empty)
{
    lake::string_view sv;
    EXPECT_TRUE(sv.empty());
    EXPECT_EQ(sv.size(), 0);
    EXPECT_EQ(sv, "");
}

TEST(StringView, Basic)
{
    lake::string_view sv = "foo";
    EXPECT_FALSE(sv.empty());
    EXPECT_EQ(sv.size(), 3);
    EXPECT_EQ(sv, "foo");
    EXPECT_EQ(sv, sv);
}

TEST(StringView, FromCString)
{
    char const* cstr = "This is a string.";
    lake::string_view sv { cstr };
    EXPECT_EQ(sv.size(), 17);
    EXPECT_EQ(sv, cstr);
    EXPECT_EQ(sv, lake::string_view { cstr });
    // Test pointer equality.
    EXPECT_EQ(sv.data(), cstr);
}

TEST(StringView, FromArray)
{
    lake::array<char, 3> array { 'f', 'o', 'o' };
    lake::string_view sv { array };
    EXPECT_EQ(sv, "foo");
}

TEST(StringView, FromConstArray)
{
    lake::array<char const, 3> array { 'f', 'o', 'o' };
    lake::string_view sv { array };
    EXPECT_EQ(sv, "foo");
}

TEST(StringView, FromLiteral)
{
    auto sv = "foo"_sv;
    EXPECT_EQ(sv.substring(0, sv.size()), sv);
    EXPECT_EQ(sv.substring(1, 2), "oo");

    EXPECT_DEATH(std::ignore = sv.substring(0, 4), "");
    EXPECT_DEATH(std::ignore = sv.substring(10, 0), "");
}

TEST_TODO(StringView, Contains)
TEST_TODO(StringView, StartsWithEndsWith)
TEST_TODO(StringView, Equality)
