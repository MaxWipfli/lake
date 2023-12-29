/*
 * SPDX-FileCopyrightText: 2022 Max Wipfli <mail@maxwipfli.ch>
 * SPDX-LicenseIdentifier: MIT
 */

#include <gtest/gtest.h>
#include <lake/span.hpp>
#include <lake/types.hpp>

TEST(Span, Empty)
{
    lake::span<u8> span;
    EXPECT_EQ(span.size(), 0);
    EXPECT_TRUE(span.empty());
    EXPECT_EQ(span.data(), nullptr);
    EXPECT_EQ(span.begin(), span.end());

    for (auto item : span) {
        // This should be unreachable.
        EXPECT_TRUE(false);
    }
}

TEST(Span, Basic)
{
    u8 data[2] = { 1, 2 };
    lake::span<u8> span(data);
    EXPECT_EQ(span.size(), 2);
    EXPECT_EQ(span.data(), data);

    EXPECT_EQ(span.at(0), 1);
    EXPECT_EQ(span[0], 1);
    EXPECT_EQ(span.at(1), 2);
    EXPECT_EQ(span.at(1), 2);
}

TEST(Span, FrontBack)
{
    lake::span<u8> span;
    EXPECT_DEATH((void)span.front(), "");
    EXPECT_DEATH((void)span.back(), "");

    u8 data[3] = { 1, 2, 3 };
    span = data;
    EXPECT_EQ(&span.front(), &span[0]);
    EXPECT_EQ(&span.back(), &span[2]);
}

TEST(Span, Iterators)
{
    u8 data[2] = { 1, 2 };
    lake::span<u8> span(data);

    auto it = span.begin();
    EXPECT_NE(it, span.end());
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_NE(it, span.begin());
    EXPECT_NE(it, span.end());
    EXPECT_EQ(*it, 2);
    ++it;
    EXPECT_NE(it, span.begin());
    EXPECT_EQ(it, span.end());

    // Iterators in range-based for-loop.
    u8 count = 0;
    u8 sum = 0;
    for (auto value : span) {
        count += 1;
        sum += value;
    }
    EXPECT_EQ(count, 2);
    EXPECT_EQ(sum, 3);
}

TEST(Span, OutOfBoundsAccess)
{
    u8 data[2] = { 1, 2 };
    lake::span<u8> span(data);

    EXPECT_DEATH((void)span.at(2), "");
    EXPECT_DEATH((void)span[2], "");
    EXPECT_DEATH((void)span.at(-1), "");
    EXPECT_DEATH((void)span[-1], "");
}

TEST(Span, Equality)
{
    u8 data[3] = { 1, 2, 3 };
    u8 same_data[2] = { 1, 2 };
    u8 different_data[2] = { 4, 5 };
    lake::span<u8> span2(data, 2);
    lake::span<u8> span3(data, 3);
    lake::span<u8> span3_second(data, 3);
    lake::span<u8> span2_same_data(same_data, 2);
    lake::span<u8> span2_different_data(different_data, 2);

    // Same span object.
    EXPECT_TRUE(span3 == span3);
    EXPECT_FALSE(span3 != span3);

    // Different span object pointing to same data.
    EXPECT_TRUE(span3 == span3_second);
    EXPECT_FALSE(span3 != span3_second);

    // Same data, but different size.
    EXPECT_FALSE(span2 == span3);
    EXPECT_TRUE(span2 != span3);
    EXPECT_FALSE(span3 == span2);
    EXPECT_TRUE(span3 != span2);

    // Different data, but elements compare equal.
    EXPECT_TRUE(span2 == span2_same_data);
    EXPECT_FALSE(span2 != span2_same_data);

    // Same length, but different data.
    EXPECT_FALSE(span2 == span2_different_data);
    EXPECT_TRUE(span2 != span2_different_data);

    // Array constructor.
    lake::span<u8> span3_array_constructor(data);
    EXPECT_TRUE(span3 == span3_array_constructor);
    EXPECT_FALSE(span3 != span3_array_constructor);
}

TEST(Span, Subspan)
{
    u8 data[2] = { 1, 2 };
    lake::span<u8> span(data, sizeof(data) / sizeof(data[0]));

    auto subspan_full = span.subspan(0, 2);
    EXPECT_EQ(subspan_full.data(), span.data());
    EXPECT_EQ(subspan_full.size(), span.size());
    EXPECT_EQ(subspan_full, span);

    auto subspan_empty = span.subspan(0, 0);
    // We don't required subspan_empty.data() == span.data(), as this is just an implementation detail.
    EXPECT_EQ(subspan_empty.size(), 0);
    EXPECT_NE(subspan_empty, subspan_full);
    EXPECT_NE(subspan_empty, span);

    auto subspan_empty2 = span.subspan(1, 0);
    // Empty spans of the same (or comparable) types should compare equal.
    EXPECT_EQ(subspan_empty, subspan_empty2);

    auto subspan_real = span.subspan(1, 1);
    EXPECT_EQ(subspan_real.size(), 1);
    EXPECT_EQ(&subspan_real[0], &span[1]);

    // Out-of-bounds subspan creation should fail.
    EXPECT_DEATH((void)span.subspan(1, 2), "");
    EXPECT_DEATH((void)span.subspan(4, 0), "");
    // start + size overflow to be inside the valid range.
    EXPECT_DEATH((void)span.subspan((size_t)-1, 2), "");
}

TEST(Span, FromInitializerList)
{
    lake::span<int const> span = { 1, 2, 3 };
    EXPECT_EQ(span.size(), 3);
    EXPECT_EQ(span[0], 1);
    EXPECT_EQ(span[1], 2);
    EXPECT_EQ(span[2], 3);
}

// FIXME: Test constexpr functionality.
