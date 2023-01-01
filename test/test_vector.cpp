/*
 * SPDX-FileCopyrightText: 2023 Max Wipfli <mail@maxwipfli.ch>
 * SPDX-LicenseIdentifier: MIT
 */

#include "utils.hpp"
#include <gtest/gtest.h>
#include <lake/array.hpp>
#include <lake/vector.hpp>

TEST(Vector, Empty)
{
    lake::vector<u16> vec;
    EXPECT_EQ(vec.size(), 0);
    EXPECT_TRUE(vec.empty());
    // The default constructor must not allocate.
    EXPECT_EQ(vec.capacity(), 0);
    EXPECT_EQ(vec.begin(), vec.end());
    EXPECT_EQ(vec.span(), lake::span<u16>());

    EXPECT_DEATH((void)vec[0], "");
    EXPECT_DEATH((void)vec.front(), "");
    EXPECT_DEATH((void)vec.back(), "");
}

TEST(Vector, Basic)
{
    lake::vector<u16> vec;
    vec.push_back((u16)1);
    vec.push_back(2);
    vec.push_back(3);

    EXPECT_EQ(vec.size(), 3);
    EXPECT_TRUE(vec.capacity() >= vec.size());
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 3);
    EXPECT_EQ(vec.span().size(), 3);

    vec.pop_back();
    vec.pop_back();

    EXPECT_EQ(vec.size(), 1);
    EXPECT_TRUE(vec.capacity() >= vec.size());
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec.span().size(), 1);
}

TEST(Vector, NonCopyable)
{
    lake::vector<non_copyable> vec;
    for (size_t i = 0; i < 16; ++i) {
        vec.push_back(non_copyable());
    }

    auto moved_out_element = lake::move(vec.back());

    // This test only needs to compile for success, as it solely checks that no unexpected
    // copies are made. For good measure, check we have the correct dummy value.
    EXPECT_EQ(moved_out_element.dummy(), non_copyable::expected_dummy());
}

TEST(Vector, DestructionCounts)
{
    int destruction_count = 0;

    {
        lake::vector<destruction_counter> vec;
        for (int i = 0; i < 8; ++i) {
            vec.emplace_back(&destruction_count);
        }
    }

    EXPECT_EQ(destruction_count, 8);
}

TEST(Vector, Filled)
{
    constexpr size_t count = 10;
    constexpr int value = 42;

    auto int_vec = lake::vector<int>::filled(count, value);
    EXPECT_EQ(int_vec.size(), count);
    EXPECT_TRUE(int_vec.size() <= int_vec.capacity());
    for (auto const& elem : int_vec) {
        EXPECT_EQ(elem, value);
    }

    int destruction_count = 0;
    {
        auto counter = destruction_counter(&destruction_count);
        auto vec = lake::vector<destruction_counter>::filled(10, counter);
        EXPECT_EQ(vec.size(), count);
    }
    // One for each of the vector elements, plus one for `counter`.
    EXPECT_EQ(destruction_count, count + 1);
}

TEST(Vector, FromSpan)
{
    lake::array<int, 4> data = { 1, 2, 3, 4 };
    lake::vector<int> vec = data.span();
    EXPECT_EQ(vec.size(), data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        EXPECT_EQ(data[i], vec[i]);
    }
}

TEST(Vector, PopBack)
{
    int destruction_count = 0;
    {
        lake::vector<destruction_counter> vec;
        vec.emplace_back(&destruction_count);
        vec.emplace_back(&destruction_count);
        vec.emplace_back(&destruction_count);
        vec.pop_back();
        vec.pop_back();
        EXPECT_EQ(destruction_count, 2);
    }
    // Two elements are destructed upon `pop_back`, and the remaining one by the destructor.
    EXPECT_EQ(destruction_count, 3);
}

TEST(Vector, Equality)
{
    lake::array<u8, 4> array = { 1, 2, 3, 4 };
    lake::vector<u8> vector = array.span();
    lake::vector<u8> another_vector = array.span();
    EXPECT_EQ(vector, another_vector);
    EXPECT_EQ(vector, array.span());
}
