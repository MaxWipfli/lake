/*
 * SPDX-FileCopyrightText: 2022-2023 Max Wipfli <mail@maxwipfli.ch>
 * SPDX-LicenseIdentifier: MIT
 */

#pragma once

namespace lake {

// remove_const
template <typename T>
struct remove_const {
    using type = T;
};
template <typename T>
struct remove_const<T const> {
    using type = T;
};

template <typename T>
using remove_const_t = typename remove_const<T>::type;

// remove_reference
template <typename T>
struct remove_reference {
    using type = T;
};
template <typename T>
struct remove_reference<T&> {
    using type = T;
};
template <typename T>
struct remove_reference<T&&> {
    using type = T;
};

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;

}
