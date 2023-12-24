/*
 * SPDX-FileCopyrightText: 2023 Max Wipfli <mail@maxwipfli.ch>
 * SPDX-LicenseIdentifier: MIT
 */

#pragma once

#include "array.hpp"
#include "extras.hpp"
#include "span.hpp"
#include "types.hpp"
#include <assert.h>

namespace lake {

class string_view : public span<char const> {
public:
    constexpr string_view() = default;

    template <size_t Size>
    constexpr string_view(char const array[Size]) // NOLINT(google-explicit-constructor)
        : span(array, Size)
    {
    }

    constexpr string_view(char const* data, size_t size)
        : span(data, size)
    {
    }

    template <size_t Size>
    constexpr string_view(array<char, Size> const& array) // NOLINT(google-explicit-constructor)
        : span(array.data(), array.size())
    {
    }

    template <size_t Size>
    constexpr string_view(array<char const, Size> const& array) // NOLINT(google-explicit-constructor)
        : span(array.data(), array.size())
    {
    }

    constexpr string_view(char const* cstr) // NOLINT(google-explicit-constructor)
        : span(cstr, __builtin_strlen(cstr))
    {
    }

    // Substring: Only the const version is implemented the data pointed to by a string view is immutable.
    [[nodiscard]] constexpr string_view substring(size_t start, size_t size) const
    {
        assert(start <= this->size() && (start + size) <= this->size());
        return { this->data() + start, size };
    }

    // Utility functions
    [[nodiscard]] constexpr bool contains(string_view needle) const;
    [[nodiscard]] constexpr bool contains(char needle) const;
    [[nodiscard]] constexpr bool starts_with(string_view prefix) const;
    [[nodiscard]] constexpr bool ends_with(string_view suffix) const;

    constexpr bool operator==(string_view const& other) const
    {
        if (this == &other) {
            return true;
        }
        if (this->size() != other.size()) {
            return false;
        }
        if (this->data() == other.data()) {
            return true;
        }
        return __builtin_memcmp(this->data(), other.data(), this->size()) == 0;
    }
};

}

constexpr lake::string_view operator""_sv(char const* data, size_t size)
{
    return { data, size };
}
