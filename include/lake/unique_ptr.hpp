/*
 * SPDX-FileCopyrightText: 2023 Max Wipfli <mail@maxwipfli.ch>
 * SPDX-LicenseIdentifier: MIT
 */

#pragma once

#include "extras.hpp"

namespace lake {

template <typename T>
class unique_ptr {
public:
    static unique_ptr<T> adopt(T* ptr)
    {
        return unique_ptr<T>(ptr);
    }

    unique_ptr() = default;

    ~unique_ptr()
    {
        clear();
    }

    unique_ptr(unique_ptr const&) = delete;

    unique_ptr(unique_ptr&& other) noexcept
        : m_ptr(other.m_ptr)
    {
        other.m_ptr = nullptr;
    }

    unique_ptr& operator=(unique_ptr const& other) = delete;

    unique_ptr& operator=(unique_ptr&& other) noexcept
    {
        clear();
        m_ptr = other.m_ptr;
        other.m_ptr = nullptr;
        return *this;
    }

    T* ptr() { return m_ptr; }
    T const* ptr() const { return m_ptr; }

    T* operator*() { return m_ptr; }
    T const* operator*() const { return m_ptr; }
    T* operator->() { return m_ptr; }
    T const* operator->() const { return m_ptr; }

    operator bool() const { return m_ptr; } // NOLINT(google-explicit-constructor)

    template <typename... Args>
    void emplace(Args... args)
    {
        clear();
        m_ptr = new T(args...);
    }

    void clear()
    {
        delete m_ptr;
        m_ptr = nullptr;
    }

private:
    explicit unique_ptr(T* ptr)
        : m_ptr(ptr)
    {
    }

    T* m_ptr { nullptr };
};

template <typename T>
unique_ptr<T> adopt_unique(T* ptr)
{
    return unique_ptr<T>::adopt(ptr);
}

template <typename T, typename... Args>
unique_ptr<T> make_unique(Args... args)
{
    T* ptr = new T(args...);
    return unique_ptr<T>::adopt(ptr);
}

}
