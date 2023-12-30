/*
 * SPDX-FileCopyrightText: 2023 Max Wipfli <mail@maxwipfli.ch>
 * SPDX-LicenseIdentifier: MIT
 */

#pragma once

#include "extras.hpp"
#include "iterator.hpp"
#include "string_view.hpp"
#include "unique_ptr.hpp"
#include <stdlib.h>

namespace lake {

template <typename T>
class fixed_array {
public:
    fixed_array() = default;

    // span (copy) constructor/assignment operators
    template <typename U>
    fixed_array(lake::span<U> span) // NOLINT(google-explicit-constructor)
    {
        allocate(span.size());
        for (size_t i = 0; i < span.size(); i++) {
            // Copy-construct the elements into their storage slots.
            // Remove const-ness from T* (if present) to allow modifying the slot.
            auto* slot = const_cast<remove_const_t<T>*>(&m_storage->data[i]);
            new (slot) T(span[i]);
        }
    }

    template <typename U>
    fixed_array<T>& operator=(lake::span<U> span)
    {
        // TODO: Reuse storage if the new data has the same number of elements.
        clear();
        allocate(span.size());

        for (size_t i = 0; i < span.size(); i++) {
            // Copy-construct the elements into their storage slots.
            new (&m_storage->data[i]) T(span.at(i));
        }
        return *this;
    }

    // initializer list constructor/assignment operator (via span)
    template <typename U>
    fixed_array(std::initializer_list<U> initializer_list)
        : fixed_array(lake::span<U const>(initializer_list))
    {
    }
    template <typename U>
    fixed_array<T>& operator=(std::initializer_list<U> initializer_list)
    {
        *this = lake::span<U const>(initializer_list);
        return *this;
    }

    // copy constructor/assignment operator (via span constructor/assignment operator)
    fixed_array(fixed_array<T> const& other)
        : fixed_array(other.span())
    {
    }
    fixed_array<T>& operator=(fixed_array<T> const& other)
    {
        if (this == &other) {
            // self-assignment
            return *this;
        }

        *this = other.span();
        return *this;
    }

    // move constructor/assignment operator
    fixed_array(fixed_array<T>&& other) noexcept
        : m_storage(other.m_storage)
    {
        other.m_storage = nullptr;
    }
    fixed_array<T>& operator=(fixed_array<T>&& other) noexcept
    {
        clear();
        m_storage = exchange(other.m_storage, nullptr);
        return *this;
    }

    // destructor
    ~fixed_array() { clear(); }

    void clear()
    {
        if (!m_storage) {
            return;
        }

        for (size_t i = 0; i < m_storage->size; i++) {
            m_storage->data[i].~T();
        }

        free(m_storage);
        m_storage = nullptr;
    }

    // basics
    [[nodiscard]] T* data() { return m_storage ? m_storage->data : nullptr; }
    [[nodiscard]] T const* data() const { return m_storage ? m_storage->data : nullptr; }
    [[nodiscard]] size_t size() const { return m_storage ? m_storage->size : 0; }
    [[nodiscard]] bool empty() const { return !m_storage; }

    // spans
    [[nodiscard]] lake::span<T> span() { return { data(), size() }; }
    [[nodiscard]] lake::span<T const> span() const { return { data(), size() }; }
    [[nodiscard]] lake::span<T> subspan(size_t start, size_t size) { return span().subspan(start, size); }
    [[nodiscard]] lake::span<T const> subspan(size_t start, size_t size) const { return span().subspan(start, size); }
    [[nodiscard]] operator lake::span<T>() { return span(); } // NOLINT(google-explicit-constructor)
    [[nodiscard]] operator lake::span<T const>() const { return span(); } // NOLINT(google-explicit-constructor)

    // element access
    [[nodiscard]] T& at(size_t index)
    {
        assert(index < size());
        return m_storage->data[index];
    }
    [[nodiscard]] T const& at(size_t index) const
    {
        assert(index < size());
        return m_storage->data[index];
    }
    [[nodiscard]] T& operator[](size_t index) { return at(index); }
    [[nodiscard]] T const& operator[](size_t index) const { return at(index); }
    [[nodiscard]] T& front() { return at(0); }
    [[nodiscard]] T const& front() const { return at(0); }
    [[nodiscard]] T& back() { return at(size() - 1); }
    [[nodiscard]] T const& back() const { return at(size() - 1); }

    // iterators
    using iterator = contiguous_iterator<T>;
    using const_iterator = contiguous_iterator<T const>;
    [[nodiscard]] iterator begin() { return iterator(data()); }
    [[nodiscard]] const_iterator begin() const { return const_iterator(data()); }
    [[nodiscard]] iterator end() { return iterator(data() + size()); }
    [[nodiscard]] const_iterator end() const { return const_iterator(data() + size()); }

    // equality operators
    template <typename U>
    bool operator==(fixed_array<U> const& other) const
    {
        if (this == &other) {
            return true;
        }
        return span() == other.span();
    }
    template <typename U>
    bool operator==(lake::span<U> const& other) const
    {
        return span() == other;
    }

private:
    // allocate storage for certain size, with elements remaining uninitialized
    void allocate(size_t size)
    {
        assert(!m_storage);

        if (size == 0) {
            // Skip allocation for zero size.
            return;
        }

        auto storage_ptr = malloc(storage::allocation_bytes(size));
        assert(storage_ptr);
        m_storage = static_cast<storage*>(storage_ptr);
        m_storage->size = size;
    }

    // Storage for the fixed_array. This needs to always be allocated with the number of bytes returned by
    // allocation_bytes(size). Once constructed and initialized, it is expected that `data` contains exactly `size`
    // initialized values of type T.
    struct storage {
        static size_t allocation_bytes(size_t size)
        {
            return sizeof(storage) + size * sizeof(T);
        }

        size_t size { 0 };
        T data[0];
    };

    // If m_storage == nullptr, then there are 0 elements.
    storage* m_storage { nullptr };
};

static_assert(sizeof(fixed_array<size_t>) == sizeof(void*), "sizeof(fixed_array<T>) == pointer size");

}
