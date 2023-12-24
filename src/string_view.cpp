/*
 * SPDX-FileCopyrightText: 2023 Max Wipfli <mail@maxwipfli.ch>
 * SPDX-LicenseIdentifier: MIT
 */

#include "string_view.hpp"

namespace lake {

constexpr bool string_view::contains(string_view needle) const
{
    if (needle.size() > this->size()) {
        return false;
    }
    for (size_t i = 0; i < this->size() + 1 - needle.size(); i++) {
        if (this->substring(i, needle.size()) == needle) {
            return true;
        }
    }
    return false;
}

constexpr bool string_view::contains(char needle) const
{
    for (auto c : *this) {
        if (c == needle) {
            return true;
        }
    }
    return false;
}

constexpr bool string_view::starts_with(string_view prefix) const
{
    if (prefix.size() > this->size()) {
        return false;
    }
    return this->substring(0, prefix.size()) == prefix;
}

constexpr bool string_view::ends_with(string_view suffix) const
{
    if (suffix.size() > this->size()) {
        return false;
    }
    return this->substring(this->size() - suffix.size(), suffix.size()) == suffix;
}

}
