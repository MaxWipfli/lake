/*
 * SPDX-FileCopyrightText: 2023 Max Wipfli <mail@maxwipfli.ch>
 * SPDX-LicenseIdentifier: MIT
 */

#include "utils.hpp"
#include <gtest/gtest.h>
#include <lake/optional.hpp>

TEST(Optional, Empty)
{
    lake::optional<non_copyable> opt;
    EXPECT_FALSE(opt.has_value());
    EXPECT_DEATH((void)opt.value(), "");

    opt.clear();
    EXPECT_FALSE(opt.has_value());
}

TEST(Optional, Basic)
{
    lake::optional<non_copyable> opt = non_copyable();
    EXPECT_TRUE(opt.has_value());
    EXPECT_EQ(&opt.value(), &(*opt));
    EXPECT_EQ(opt.value().dummy(), non_copyable::expected_dummy());
    EXPECT_EQ(opt->dummy(), non_copyable::expected_dummy());
    opt.clear();
    EXPECT_FALSE(opt.has_value());
}

TEST(Optional, EmplaceRelease)
{
    {
        lake::optional<non_copyable_movable> opt;
        opt.emplace();
        EXPECT_TRUE(opt.has_value());
        EXPECT_EQ(opt->dummy(), non_copyable_movable::expected_dummy());
        opt.clear();
        EXPECT_FALSE(opt.has_value());
    }
}

TEST(Optional, DestructionCounts)
{
    int destruction_count = 0;
    {
        lake::optional<destruction_counter> opt(&destruction_count);
    }
    EXPECT_EQ(destruction_count, 1);

    destruction_count = 0;
    {
        lake::optional<destruction_counter> opt(&destruction_count);
        auto value = opt.release_value();
        (void)value;
    }
    EXPECT_EQ(destruction_count, 1);

    destruction_count = 0;
    {
        lake::optional<destruction_counter> opt(&destruction_count);
        // Make a copy of `value`.
        destruction_counter value = opt.value();
        (void)value;
    }
    EXPECT_EQ(destruction_count, 2);

    destruction_count = 0;
    {
        lake::optional<destruction_counter> opt(&destruction_count);
        // Don't make a copy, just take a reference.
        auto const& value = opt.value();
        (void)value;
    }
    EXPECT_EQ(destruction_count, 1);
}

TEST(Optional, Equality)
{
    lake::optional<u8> first = 3;
    lake::optional<u8> second = 3;
    lake::optional<u8> different = 4;
    lake::optional<u8> empty;
    lake::optional<u8> another_empty;

    EXPECT_TRUE(first == second);
    EXPECT_FALSE(first != second);

    EXPECT_FALSE(first == different);
    EXPECT_TRUE(first != different);
    EXPECT_FALSE(different == first);

    EXPECT_FALSE(first == empty);
    EXPECT_TRUE(first != empty);
    EXPECT_FALSE(empty == first);

    EXPECT_TRUE(empty == another_empty);
    EXPECT_FALSE(empty != another_empty);
}

struct three_member_struct {
    size_t foo;
    size_t bar;
    size_t baz;
};

TEST(Optional, Alignment)
{
    three_member_struct data {
        .foo = 1,
        .bar = 2,
        .baz = 3
    };

    lake::optional<three_member_struct> opt = lake::move(data);

    // Sanity checks
    EXPECT_EQ(opt->foo, 1);
    EXPECT_EQ(opt->bar, 2);
    EXPECT_EQ(opt->baz, 3);

    // Check alignment.
    auto struct_ptr = reinterpret_cast<size_t>(&opt.value());
    auto alignment_mask = ~(alignof(three_member_struct) - 1);
    EXPECT_TRUE((struct_ptr & alignment_mask) == struct_ptr);
}
