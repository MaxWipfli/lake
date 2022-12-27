/*
 * SPDX-FileCopyrightText: 2022 Max Wipfli <mail@maxwipfli.ch>
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

}
