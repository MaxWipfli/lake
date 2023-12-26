/*
 * SPDX-FileCopyrightText: 2022 Max Wipfli <mail@maxwipfli.ch>
 * SPDX-LicenseIdentifier: MIT
 */

#pragma once

namespace lake {

template <typename T>
class contiguous_iterator {
public:
    explicit constexpr contiguous_iterator(T* ptr)
        : m_ptr(ptr)
    {
    }

    constexpr bool operator==(contiguous_iterator<T> const& other) const
    {
        return m_ptr == other.m_ptr;
    }

    constexpr bool operator!=(contiguous_iterator<T> const& other) const
    {
        return !(*this == other);
    }

    constexpr contiguous_iterator<T>& operator++()
    {
        m_ptr++;
        return *this;
    }

    constexpr T& operator*() const
    {
        return *m_ptr;
    }

    constexpr T* operator->() const
    {
        return m_ptr;
    }

private:
    T* m_ptr;
};

}
