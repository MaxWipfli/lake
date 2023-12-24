/*
 * SPDX-FileCopyrightText: 2023 Max Wipfli <mail@maxwipfli.ch>
 * SPDX-LicenseIdentifier: MIT
 */

#pragma once

#include <lake/types.hpp>

class non_copyable {
public:
    non_copyable() = default;
    non_copyable(non_copyable const&) = delete;
    non_copyable& operator=(non_copyable const&) = delete;
    non_copyable(non_copyable&& other) = default;
    non_copyable& operator=(non_copyable&&) = default;

    [[nodiscard]] size_t dummy() const { return m_dummy; }

    static size_t expected_dummy() { return 42; }

private:
    volatile size_t m_dummy { 42 };
};

class non_movable {
public:
    non_movable() = default;
    non_movable(non_movable const&) = default;
    non_movable& operator=(non_movable const&) = default;
    non_movable(non_movable&&) = delete;
    non_movable& operator=(non_movable&&) = delete;

    [[nodiscard]] size_t dummy() const { return m_dummy; }

    static size_t expected_dummy() { return 42; }

private:
    volatile size_t m_dummy { 42 };
};

class non_copyable_movable {
public:
    non_copyable_movable() = default;
    non_copyable_movable(non_copyable_movable const&) = delete;
    non_copyable_movable& operator=(non_copyable_movable const&) = delete;
    non_copyable_movable(non_copyable_movable&&) = delete;
    non_copyable_movable& operator=(non_copyable_movable&&) = delete;

    [[nodiscard]] size_t dummy() const { return m_dummy; }

    static size_t expected_dummy() { return 42; }

private:
    volatile size_t m_dummy { 42 };
};

class destruction_counter {
public:
    explicit destruction_counter(int* count_ptr)
        : m_count_ptr(count_ptr)
    {
    }

    ~destruction_counter()
    {
        if (m_count_ptr) {
            (*m_count_ptr)++;
        }
    }

    destruction_counter(destruction_counter const&) = default;
    destruction_counter& operator=(destruction_counter const&) = default;

    destruction_counter(destruction_counter&& other) noexcept
        : m_count_ptr(other.m_count_ptr)
    {
        other.m_count_ptr = nullptr;
    }

    destruction_counter& operator=(destruction_counter&& other) noexcept
    {
        m_count_ptr = other.m_count_ptr;
        other.m_count_ptr = nullptr;
        return *this;
    }

private:
    int* m_count_ptr;
};

#define TEST_TODO(test_suite_name, test_name) \
    TEST(test_suite_name, test_name)          \
    {                                         \
        GTEST_SKIP_("TODO");                  \
    }
