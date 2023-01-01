/*
 * SPDX-FileCopyrightText: 2022-2023 Max Wipfli <mail@maxwipfli.ch>
 * SPDX-LicenseIdentifier: MIT
 */

#pragma once

#include "iterator.hpp"
#include "types.hpp"
#include <assert.h>

namespace lake {

template <typename T, size_t Size>
class array;

template <typename T>
class span {
public:
    constexpr span() = default;

    constexpr span(T* data, size_t size)
        : m_data(data)
        , m_size(size)
    {
    }

    template <size_t Size>
    constexpr span(T (&array)[Size]) // NOLINT(google-explicit-constructor)
        : m_data(array)
        , m_size(Size)
    {
    }

    template <size_t Size>
    constexpr span(array<T, Size>& array) // NOLINT(google-explicit-constructor)
        : m_data(array.data())
        , m_size(array.size())
    {
    }

    [[nodiscard]] constexpr T const* data() const
    {
        return m_data;
    }
    [[nodiscard]] constexpr T* data() { return m_data; }
    [[nodiscard]] constexpr size_t size() const { return m_size; }
    [[nodiscard]] constexpr bool empty() const { return m_size == 0; }

    // element access
    [[nodiscard]] constexpr T& at(size_t index)
    {
        assert(index < m_size);
        return m_data[index];
    }
    [[nodiscard]] constexpr T const& at(size_t index) const
    {
        assert(index < m_size);
        return m_data[index];
    }
    [[nodiscard]] constexpr T& operator[](size_t index) { return at(index); }
    [[nodiscard]] constexpr T const& operator[](size_t index) const { return at(index); }
    [[nodiscard]] constexpr T& front() { return at(0); }
    [[nodiscard]] constexpr T const& front() const { return at(0); }
    [[nodiscard]] constexpr T& back() { return at(m_size - 1); }
    [[nodiscard]] constexpr T const& back() const { return at(m_size - 1); }

    // sub-spans
    [[nodiscard]] constexpr span<T> subspan(size_t start, size_t size)
    {
        assert(start <= m_size && (start + size) <= m_size);
        return { m_data + start, size };
    }
    [[nodiscard]] constexpr span<T const> subspan(size_t start, size_t size) const
    {
        assert(start <= m_size && (start + size) <= m_size);
        return { m_data + start, size };
    }

    using iterator = lake::contiguous_iterator<T>;
    using const_iterator = lake::contiguous_iterator<T const>;
    [[nodiscard]] constexpr iterator begin() { return iterator(m_data); }
    [[nodiscard]] constexpr const_iterator begin() const { return const_iterator(m_data); }
    [[nodiscard]] constexpr iterator end() { return iterator(m_data + m_size); }
    [[nodiscard]] constexpr const_iterator end() const { return const_iterator(m_data + m_size); }

    template <typename U>
    constexpr bool operator==(span<U> const& other) const
    {
        if (this->size() != other.size()) {
            return false;
        }
        if (this->data() == other.data()) {
            // The span objects point to the same data.
            return true;
        }
        for (size_t i = 0; i < m_size; ++i) {
            if (this->at(i) != other.at(i)) {
                return false;
            }
        }
        return true;
    }

private:
    T* m_data { nullptr };
    size_t m_size { 0 };
};

}
