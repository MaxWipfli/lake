/*
 * SPDX-FileCopyrightText: 2022-2023 Max Wipfli <mail@maxwipfli.ch>
 * SPDX-LicenseIdentifier: MIT
 */

#pragma once

#include "type_traits.hpp"

namespace lake {

template <typename T>
constexpr T&& forward(remove_reference_t<T>& value) noexcept
{
    return static_cast<T&&>(value);
}
template <typename T>
constexpr T&& forward(remove_reference_t<T>&& value) noexcept
{
    return static_cast<T&&>(value);
}

template <typename T>
constexpr remove_reference_t<T>&& move(T&& value) noexcept
{
    return static_cast<remove_reference_t<T>&&>(value);
}

template <typename T>
constexpr void swap(T& first, T& second)
{
    T tmp = move(first);
    first = move(second);
    second = move(tmp);
}

template <typename T, typename U>
constexpr T exchange(T& object, U&& new_value)
{
    auto tmp = move(object);
    object = new_value;
    return tmp;
}

// FIXME: This should only work for unsigned integer types.
template <typename T>
constexpr T bit_ceil(T value)
{
    T ret = 1;
    while (ret < value) {
        ret *= 2;
    }
    return ret;
}

// FIXME: This should only work for unsigned integer types.
template <typename T>
constexpr T bit_floor(T value)
{
    if (value == 0) {
        return 0;
    }
    T ceil = bit_ceil(value);
    if (value != ceil) {
        ceil /= 2;
    }
    return ceil;
}

}
