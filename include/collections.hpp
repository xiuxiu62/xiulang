#pragma once

#include "defines.hpp"

struct pool_handle {
    u32 index;

    static constexpr pool_handle invalid() {
        return {u32_max};
    }

    constexpr bool operator==(pool_handle other) const {
        return index == other.index;
    }

    constexpr bool operator!=(pool_handle other) const {
        return index != other.index;
    }

    constexpr bool is_valid() const {
        return *this == pool_handle::invalid();
    }

    constexpr bool is_invalid() const {
        return *this == pool_handle::invalid();
    }
};

template <typename T> struct pool {
    T *data = nullptr;
    u32 size = 0;
    u32 capacity = 0;

    bool init(u32 initial_capacity = 0);
    void deinit();

    pool_handle push(T item);
    T *get(pool_handle handle);
    const T *get(pool_handle handle) const;
};
