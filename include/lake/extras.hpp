/*
 * SPDX-FileCopyrightText: 2022 Max Wipfli <mail@maxwipfli.ch>
 * SPDX-LicenseIdentifier: MIT
 */

#pragma once

namespace lake {

template <typename T>
constexpr T&& move(T& value)
{
    return static_cast<T&&>(value);
}

}
