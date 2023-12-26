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

    EXPECT_DEATH((void)sv.substring(0, 4), "");
    EXPECT_DEATH((void)sv.substring(10, 0), "");
}

TEST(StringView, ContainsChar)
{
    auto sv = "foo bar baz"_sv;

    EXPECT_TRUE(sv.contains('f')); // first
    EXPECT_TRUE(sv.contains(' ')); // middle
    EXPECT_TRUE(sv.contains('z')); // last

    EXPECT_FALSE(sv.contains('x'));
    EXPECT_FALSE(sv.contains('\0'));
}

TEST(StringView, ContainsStringView)
{
    auto sv = "foo bar baz"_sv;

    EXPECT_TRUE(sv.contains("f"_sv));
    EXPECT_TRUE(sv.contains("foo"_sv));
    EXPECT_TRUE(sv.contains("oo bar"_sv));
    EXPECT_TRUE(sv.contains("baz"_sv));
    EXPECT_TRUE(sv.contains(sv));

    EXPECT_FALSE(sv.contains("foobar"_sv));
    EXPECT_FALSE(sv.contains("bazi"_sv));
    EXPECT_FALSE(sv.contains("baz\0"_sv));
}

TEST(StringView, StartsWith)
{
    auto sv = "foo bar baz"_sv;

    EXPECT_TRUE(sv.starts_with(""_sv));
    EXPECT_TRUE(sv.starts_with("f"_sv));
    EXPECT_TRUE(sv.starts_with("foo"_sv));
    EXPECT_TRUE(sv.starts_with(sv));

    EXPECT_FALSE(sv.starts_with("oo"_sv));
    EXPECT_FALSE(sv.starts_with("baz"_sv));
    EXPECT_FALSE(sv.starts_with("foo bar baz foo"_sv));
}

TEST(StringView, EndsWidth)
{
    auto sv = "foo bar baz"_sv;
    EXPECT_TRUE(sv.ends_with(""_sv));
    EXPECT_TRUE(sv.ends_with("z"_sv));
    EXPECT_TRUE(sv.ends_with("baz"_sv));
    EXPECT_TRUE(sv.ends_with("foo bar baz"_sv));

    EXPECT_FALSE(sv.ends_with("ba"_sv));
    EXPECT_FALSE(sv.ends_with("aaa foo bar baz"_sv));
    EXPECT_FALSE(sv.ends_with("baz\0"_sv));
}
