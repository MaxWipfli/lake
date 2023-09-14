/*
 * SPDX-FileCopyrightText: 2023 Max Wipfli <mail@maxwipfli.ch>
 * SPDX-LicenseIdentifier: MIT
 */

#pragma once

#include "extras.hpp"
#include "types.hpp"
#include <assert.h>

namespace lake {

template <typename T>
class optional {
public:
    // Don't use `= default` because this allows us to enable the uninitialized `m_slot` warning here
    // instead of on the first line of the class definition, since it belongs here.
    optional() {}; // NOLINT(cppcoreguidelines-pro-type-member-init,modernize-use-equals-default)

    ~optional()
    {
        clear();
    }

    template <typename U>
    optional(U const& value) // NOLINT(google-explicit-constructor,cppcoreguidelines-pro-type-member-init)
    {
        m_has_value = true;
        new (slot()) T(forward<U const>(value));
    }

    optional(T&& value) // NOLINT(google-explicit-constructor,cppcoreguidelines-pro-type-member-init)
    {
        m_has_value = true;
        new (slot()) T(forward<T>(value));
    }

    template <typename... Args>
    optional(Args... args) // NOLINT(google-explicit-constructor,cppcoreguidelines-pro-type-member-init)
    {
        m_has_value = true;
        new (slot()) T(forward<Args>(args)...);
    }

    template <typename U>
    optional(optional<U> const& other) // NOLINT(google-explicit-constructor,cppcoreguidelines-pro-type-member-init)
    {
        m_has_value = other.m_has_value;
        if (m_has_value) {
            new (slot()) T(forward<U const>(*other));
        }
    }
    optional(optional<T>&& other) noexcept // NOLINT(google-explicit-constructor,cppcoreguidelines-pro-type-member-init)
    {
        m_has_value = other.m_has_value;
        if (m_has_value) {
            new (slot()) T(move(*other));
            other.value().~T();
            other.m_has_value = false;
        }
    }
    template <typename U>
    optional<T>& operator=(optional<U> const& other)
    {
        if (!other.m_has_value) {
            clear();
            return *this;
        }
        if (m_has_value) {
            value() = forward<U const>(*other);
        } else {
            new (slot()) T(forward<U const>(*other));
            m_has_value = true;
        }
        return *this;
    }
    optional<T>& operator=(optional<T>&& other) noexcept
    {
        if (!other.m_has_value) {
            clear();
            return *this;
        }
        if (m_has_value) {
            value() = move(*other);
        } else {
            new (slot()) T(move(*other));
            m_has_value = true;
        }
    }

    [[nodiscard]] bool has_value() const { return m_has_value; }
    [[nodiscard]] T& value()
    {
        assert(m_has_value);
        return *slot();
    }
    [[nodiscard]] T const& value() const
    {
        assert(m_has_value);
        return *slot();
    }

    template <typename... Args>
    void emplace(Args... args)
    {
        clear();
        new (slot()) T(forward<Args>(args)...);
        m_has_value = true;
    }

    void clear()
    {
        if (!m_has_value) {
            return;
        }
        value().~T();
        m_has_value = false;
    }

    [[nodiscard]] T release_value()
    {
        assert(m_has_value);
        T tmp = move(value());
        clear();
        return tmp;
    }

    T& operator*() { return value(); }
    T const& operator*() const { return value(); }
    T* operator->() { return &value(); }
    T const* operator->() const { return &value(); }

    template <typename U>
    bool operator==(optional<U> const& other) const
    {
        if (this == &other) {
            return true;
        }
        if (!has_value() && !other.has_value()) {
            return true;
        }
        if (has_value() && other.has_value()) {
            return value() == other.value();
        }
        return false;
    }

private:
    T* slot() { return reinterpret_cast<T*>(&m_slot); }
    T const* slot() const { return reinterpret_cast<T const*>(&m_slot); }

    alignas(T) u8 m_slot[sizeof(T)];
    bool m_has_value { false };
};

}
