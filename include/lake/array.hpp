/*
 * SPDX-FileCopyrightText: 2022 Max Wipfli <mail@maxwipfli.ch>
 * SPDX-LicenseIdentifier: MIT
 */

#pragma once

#include "extras.hpp"
#include "span.hpp"
#include "types.hpp"
#include <assert.h>

namespace lake {

template <typename T, size_t Size>
struct array {
    [[nodiscard]] constexpr static array<T, Size> filled(T const& value)
    {
        array<T, Size> arr;
        arr.fill(value);
        return arr;
    }

    [[nodiscard]] constexpr T* data() { return __data; }
    [[nodiscard]] constexpr T const* data() const { return __data; }
    [[nodiscard]] constexpr size_t size() const { return Size; }
    [[nodiscard]] constexpr bool empty() const { return Size == 0; }

    constexpr void fill(T const& value)
    {
        for (T& element : *this) {
            element = value;
        }
    }

    // spans
    [[nodiscard]] constexpr ::lake::span<T> span() { return *this; }
    [[nodiscard]] constexpr ::lake::span<T const> span() const { return *this; }
    [[nodiscard]] constexpr ::lake::span<T> subspan(size_t start, size_t size) { return span().subspan(start, size); }
    [[nodiscard]] constexpr ::lake::span<T const> subspan(size_t start, size_t size) const { return span().subspan(start, size); }

    // element access
    [[nodiscard]] constexpr T& at(size_t index)
    {
        assert(index < Size);
        return __data[index];
    }
    [[nodiscard]] constexpr T const& at(size_t index) const
    {
        assert(index < Size);
        return __data[index];
    }
    [[nodiscard]] constexpr T& operator[](size_t index) { return at(index); }
    [[nodiscard]] constexpr T const& operator[](size_t index) const { return at(index); }
    [[nodiscard]] constexpr T& front() { return at(0); }
    [[nodiscard]] constexpr T const& front() const { return at(0); }
    [[nodiscard]] constexpr T& back() { return at(Size - 1); }
    [[nodiscard]] constexpr T const& back() const { return at(Size - 1); }

    // iterators
    using iterator = lake::contiguous_iterator<T>;
    using const_iterator = lake::contiguous_iterator<T const>;
    [[nodiscard]] constexpr iterator begin() { return iterator(__data); }
    [[nodiscard]] constexpr const_iterator begin() const { return const_iterator(__data); }
    [[nodiscard]] constexpr iterator end() { return iterator(__data + Size); }
    [[nodiscard]] constexpr const_iterator end() const { return const_iterator(__data + Size); }

    // comparison operators
    template <typename U, size_t SizeU>
    constexpr bool operator==(array<U, SizeU> const& other) const
    {
        return this->span() == other.span();
    }
    template <typename U>
    constexpr bool operator==(::lake::span<U> const& other) const
    {
        return this->span() == other;
    }
    template <typename U, size_t SizeU>
    constexpr bool operator!=(array<U, SizeU> const& other) const
    {
        return !operator==(other);
    }
    template <typename U>
    constexpr bool operator!=(::lake::span<U> const& other) const
    {
        return !operator==(other);
    }

    T __data[Size]; // NOLINT(bugprone-reserved-identifier)
};

}
