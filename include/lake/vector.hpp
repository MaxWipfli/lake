/*
 * SPDX-FileCopyrightText: 2023 Max Wipfli <mail@maxwipfli.ch>
 * SPDX-LicenseIdentifier: MIT
 */

#pragma once

#include "extras.hpp"
#include "iterator.hpp"
#include "span.hpp"
#include "types.hpp"
#include <assert.h>
#include <initializer_list>
#include <stdlib.h>

namespace lake {

template <typename T>
class vector {
public:
    vector() = default;

    // span (copy) constructor/assignment operators
    template <typename U>
    vector(lake::span<U> span) // NOLINT(google-explicit-constructor)
    {
        m_capacity = good_capacity(span.size());
        m_data = allocate_buffer(m_capacity);
        m_size = span.size();
        assert(m_size <= m_capacity);
        for (size_t i = 0; i < m_size; ++i) {
            new (&m_data[i]) T(span[i]);
        }
    }
    template <typename U>
    vector<T>& operator=(lake::span<U> span)
    {
        if (span.size() > m_capacity) {
            // Clear the old allocation, and reallocate from a blank slate.
            clear();
            m_capacity = good_capacity(span.size());
            m_data = allocate_buffer(m_capacity);
            m_size = span.size();
            for (size_t i = 0; i < m_size; ++i) {
                new (&m_data[i]) T(span[i]);
            }
        } else {
            // Reuse the current allocation.
            assert(m_size <= span.size());
            // Use move assignment operator for all objects that already exist.
            for (size_t i = 0; i < m_size; ++i) {
                m_data[i] = span[i];
            }
            // Use move constructor for all "new" objects.
            for (size_t i = m_size; i < span.size(); ++i) {
                new (&m_data[i]) T(span[i]);
            }
            m_size = span.size();
        }
        return *this;
    }

    // initializer list constructor/assignment operators (via span)
    template <typename U>
    vector(std::initializer_list<U> initializer_list)
        : vector(lake::span<U const>(data(initializer_list), initializer_list.size()))
    {
    }
    template <typename U>
    vector<T>& operator=(std::initializer_list<U> initializer_list)
    {
        *this = lake::span<U const>(data(initializer_list), initializer_list.size());
        return *this;
    }

    // copy constructor/assignment operator
    template <typename U>
    explicit vector(vector<U> const& other)
        : vector(other.span())
    {
    }
    template <typename U>
    vector<T>& operator=(vector<U> const& other)
    {
        *this = other.span();
        return *this;
    }

    // move constructor/assignment operator
    vector(vector<T>&& other) noexcept
        : m_data(other.m_data)
        , m_capacity(other.m_capacity)
        , m_size(other.m_size)
    {
        other.m_data = nullptr;
        other.m_capacity = 0;
        other.m_size = 0;
    }
    vector<T>& operator=(vector<T>&& other) noexcept
    {
        clear();
        m_data = exchange(other.m_data, nullptr);
        m_capacity = exchange(other.m_capacity, 0);
        m_size = exchange(other.m_size, 0);
    }

    ~vector() { clear(); }

    static vector<T> filled(size_t count, T const& value)
    {
        vector<T> vec;
        vec.reserve(count);
        for (size_t i = 0; i < count; ++i) {
            vec.push_back(value);
        }
        return vec;
    }

    [[nodiscard]] T* data() { return m_data; }
    [[nodiscard]] T const* data() const { return m_data; }
    [[nodiscard]] size_t capacity() const { return m_capacity; }
    [[nodiscard]] size_t size() const { return m_size; }
    [[nodiscard]] bool empty() const { return m_size == 0; }

    // spans
    [[nodiscard]] ::lake::span<T> span() { return { m_data, m_size }; }
    [[nodiscard]] ::lake::span<T const> span() const { return { m_data, m_size }; }
    [[nodiscard]] ::lake::span<T> subspan(size_t start, size_t size) { return span().subspan(start, size); }
    [[nodiscard]] ::lake::span<T const> subspan(size_t start, size_t size) const { return span().subspan(start, size); }
    [[nodiscard]] operator ::lake::span<T>() { return span(); } // NOLINT(google-explicit-constructor)
    [[nodiscard]] operator ::lake::span<T const>() const { return span(); } // NOLINT(google-explicit-constructor)

    // element access
    [[nodiscard]] constexpr T& at(size_t index)
    {
        assert(index < m_size);
        return m_data[index];
    }
    [[nodiscard]] constexpr T const& at(size_t index) const
    {
        assert(index < m_size);
        return m_data[index];
    }
    [[nodiscard]] constexpr T& operator[](size_t index) { return at(index); }
    [[nodiscard]] constexpr T const& operator[](size_t index) const { return at(index); }
    [[nodiscard]] constexpr T& front() { return at(0); }
    [[nodiscard]] constexpr T const& front() const { return at(0); }
    [[nodiscard]] constexpr T& back() { return at(m_size - 1); }
    [[nodiscard]] constexpr T const& back() const { return at(m_size - 1); }

    // iterators
    using iterator = lake::contiguous_iterator<T>;
    using const_iterator = lake::contiguous_iterator<T const>;
    [[nodiscard]] constexpr iterator begin() { return iterator(m_data); }
    [[nodiscard]] constexpr const_iterator begin() const { return const_iterator(m_data); }
    [[nodiscard]] constexpr iterator end() { return iterator(m_data + m_size); }
    [[nodiscard]] constexpr const_iterator end() const { return const_iterator(m_data + m_size); }

    template <typename U>
    void push_back(U const& element)
    {
        reserve(m_size + 1);
        new (&m_data[m_size]) T(forward<U const&>(element));
        ++m_size;
    }

    void push_back(T&& element)
    {
        reserve(m_size + 1);
        new (&m_data[m_size]) T(forward<T>(element));
        ++m_size;
    }

    template <typename... Args>
    void emplace_back(Args... args)
    {
        reserve(m_size + 1);
        new (&m_data[m_size]) T(forward<Args>(args)...);
        ++m_size;
    }

    void pop_back()
    {
        assert(m_size >= 1);
        --m_size;
        m_data[m_size].~T();
    }

    void reserve(size_t new_capacity)
    {
        if (new_capacity <= m_capacity)
            return;
        reallocate(new_capacity);
    }

    void clear()
    {
        if (!m_data) {
            return;
        }

        for (T& elem : *this) {
            elem.~T();
        }

        free(m_data);
        m_data = nullptr;
        m_capacity = 0;
        m_size = 0;
    }

    void swap(vector<T>& other)
    {
        ::lake::swap(m_data, other.m_data);
        ::lake::swap(m_capacity, other.m_capacity);
        ::lake::swap(m_size, other.m_size);
    }

    template <typename U>
    bool operator==(vector<U> const& other) const
    {
        return span() == other;
    }

    template <typename U>
    bool operator==(::lake::span<U> const& other) const
    {
        return span() == other;
    }

private:
    static size_t good_capacity(size_t capacity)
    {
        if (capacity == 0)
            return 0;
        return bit_ceil(capacity);
    }

    // Assumes that `capacity` is already a good capacity.
    static T* allocate_buffer(size_t capacity)
    {
        auto* ptr = calloc(capacity, sizeof(T));
        assert(ptr != nullptr);
        return static_cast<T*>(ptr);
    }

    void reallocate(size_t new_capacity)
    {
        assert(m_size <= new_capacity);
        m_capacity = good_capacity(new_capacity);
        auto* new_data = allocate_buffer(m_capacity);
        for (size_t i = 0; i < m_size; ++i) {
            // Move into new buffer.
            new (&new_data[i]) T(move(m_data[i]));
            // Destroy old object.
            m_data[i].~T();
        }
        free(m_data);
        m_data = new_data;
    }

    T* m_data { nullptr };
    size_t m_capacity { 0 };
    size_t m_size { 0 };
};
}
