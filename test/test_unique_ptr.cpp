/*
 * SPDX-FileCopyrightText: 2023 Max Wipfli <mail@maxwipfli.ch>
 * SPDX-LicenseIdentifier: MIT
 */

#include "utils.hpp"
#include <gtest/gtest.h>
#include <lake/unique_ptr.hpp>

TEST(UniquePtr, Null)
{
    lake::unique_ptr<destruction_counter> unique_ptr;
    EXPECT_EQ(unique_ptr.ptr(), nullptr);
    EXPECT_FALSE(unique_ptr);
    unique_ptr.clear();
    EXPECT_EQ(unique_ptr.ptr(), nullptr);
    EXPECT_FALSE(unique_ptr);
}

TEST(UniquePtr, Basic)
{
    int destruct_count = 0;
    {
        auto unique_ptr = lake::make_unique<destruction_counter>(&destruct_count);
        EXPECT_NE(unique_ptr.ptr(), nullptr);
        EXPECT_TRUE(unique_ptr);
        EXPECT_EQ(destruct_count, 0);
    }
    EXPECT_EQ(destruct_count, 1);
}

TEST(UniquePtr, Clear)
{
    int destruct_count = 0;
    {
        auto unique_ptr = lake::make_unique<destruction_counter>(&destruct_count);
        EXPECT_NE(unique_ptr.ptr(), nullptr);
        EXPECT_TRUE(unique_ptr);
        unique_ptr.clear();
        EXPECT_EQ(unique_ptr.ptr(), nullptr);
        EXPECT_FALSE(unique_ptr);
        EXPECT_EQ(destruct_count, 1);
    }
    EXPECT_EQ(destruct_count, 1);
}

TEST(UniquePtr, Emplace)
{
    int destruct_count = 0;
    {
        lake::unique_ptr<destruction_counter> unique_ptr;
        EXPECT_FALSE(unique_ptr);
        unique_ptr.emplace(&destruct_count);
        EXPECT_TRUE(unique_ptr);
        EXPECT_EQ(destruct_count, 0);
    }
    EXPECT_EQ(destruct_count, 1);
}

TEST(UniquePtr, Adopt)
{
    int destruct_count = 0;
    {
        auto* raw_ptr = new destruction_counter(&destruct_count);
        EXPECT_TRUE(raw_ptr);
        auto unique_ptr = lake::adopt_unique(raw_ptr);
        EXPECT_EQ(unique_ptr.ptr(), raw_ptr);
        EXPECT_EQ(destruct_count, 0);
    }
    EXPECT_EQ(destruct_count, 1);
}

TEST(UniquePtr, MoveConstructor)
{
    int destruct_count = 0;
    {
        auto first_ptr = lake::make_unique<destruction_counter>(&destruct_count);
        auto first_raw_ptr = first_ptr.ptr();
        lake::unique_ptr<destruction_counter> second_ptr = move(first_ptr);

        EXPECT_EQ(first_ptr.ptr(), nullptr);
        EXPECT_EQ(second_ptr.ptr(), first_raw_ptr);
        EXPECT_TRUE(second_ptr);
        EXPECT_EQ(destruct_count, 0);
    }
    EXPECT_EQ(destruct_count, 1);
}

TEST(UniquePtr, MoveAssignmentOperator)
{
    int destruct_count = 0;
    {
        auto first_ptr = lake::make_unique<destruction_counter>(&destruct_count);
        auto first_raw_ptr = first_ptr.ptr();
        lake::unique_ptr<destruction_counter> second_ptr;
        second_ptr = move(first_ptr);

        EXPECT_EQ(first_ptr.ptr(), nullptr);
        EXPECT_EQ(second_ptr.ptr(), first_raw_ptr);
        EXPECT_TRUE(second_ptr);
        EXPECT_EQ(destruct_count, 0);
    }
    EXPECT_EQ(destruct_count, 1);
}
