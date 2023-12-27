/*
 * SPDX-FileCopyrightText: 2023 Max Wipfli <mail@maxwipfli.ch>
 * SPDX-LicenseIdentifier: MIT
 */

#include "utils.hpp"
#include <gtest/gtest.h>
#include <lake/hash.hpp>

TEST(Hash, HashState)
{
    lake::hash_state hs;
    auto initial_value = hs.value();
    hs.hash(42);
    EXPECT_NE(hs.value(), initial_value);
}

TEST(Hash, Primitive)
{
    lake::hash_state hs;
    auto initial_value = hs.value();
    hs.hash((u64)42);
    hs.hash((i64)-1);
    hs.hash((int)-3);
    hs.hash('a');
    EXPECT_NE(hs.value(), initial_value);
}

TEST(Hash, NoCollision)
{
    lake::hash_state hs1;
    hs1.hash(41);

    lake::hash_state hs2;
    hs2.hash(42);

    EXPECT_NE(hs1.value(), hs2.value());
}
