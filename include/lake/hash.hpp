/*
 * SPDX-FileCopyrightText: 2023 Max Wipfli <mail@maxwipfli.ch>
 * SPDX-LicenseIdentifier: MIT
 */

#include "types.hpp"

#pragma once

namespace lake {

template <typename T>
struct hash;

class hash_state {
public:
    using hash_t = size_t;

    constexpr hash_state() = default;

    template <typename T>
    constexpr void hash(T const& value)
    {
        lake::hash<T> {}(*this, value);
    }

    // NOTE: This function should only ever be called by lake::hash::operator() to implement hashing for a new type
    //       from scratch.
    constexpr void combine_with(hash_t hash)
    {
        // FIXME: This slowly discards the information of the values first combined into the state.
        m_state = (m_state << 1) ^ hash;
    }

    // WARNING: This requires that the iteration order of [begin, end) is deterministic, which is not the case for
    //          e.g. unordered containers.
    template <typename It>
    constexpr void hash_range(It begin, It end)
    {
        for (auto it = begin; it != end; ++it) {
            this->hash(*it);
        }
    }

    [[nodiscard]] constexpr hash_t value() const { return m_state; }

private:
    hash_t m_state { 0 };
};

// Specialize this struct for any type to implement hashing. It is recommended to use the LAKE_HASH_DEFINE macro below
// for this.
template <typename T>
struct hash {
    constexpr void operator()(hash_state& h, T const& value);
};

#define LAKE_HASH_DEFINE(type, body)                                \
    template <>                                                     \
    struct hash<type> {                                             \
        constexpr void operator()(hash_state& h, type const& value) \
        {                                                           \
            body;                                                   \
        }                                                           \
    }
LAKE_HASH_DEFINE(u64, h.combine_with((value << 3) ^ value));
LAKE_HASH_DEFINE(i64, h.hash(static_cast<u64>(value)));
LAKE_HASH_DEFINE(u32, h.hash(static_cast<u64>(value)));
LAKE_HASH_DEFINE(i32, h.hash(static_cast<u64>(value)));
LAKE_HASH_DEFINE(u16, h.hash(static_cast<u64>(value)));
LAKE_HASH_DEFINE(i16, h.hash(static_cast<u64>(value)));
LAKE_HASH_DEFINE(u8, h.hash(static_cast<u64>(value)));
LAKE_HASH_DEFINE(i8, h.hash(static_cast<u64>(value)));
LAKE_HASH_DEFINE(char, h.hash(static_cast<u64>(value)));
#undef LAKE_HASH_DEFINE

}
