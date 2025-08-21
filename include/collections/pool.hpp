#pragma once

#include "allocators.hpp"
// #include "collections.hpp"
#include "logger.hpp"

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
        return *this != pool_handle::invalid();
    }

    constexpr bool is_invalid() const {
        return *this == pool_handle::invalid();
    }
};

template <typename T> struct pool {
    parent_allocator allocator = {};

    T *data = nullptr;
    u32 size = 0;
    u32 capacity = 0;

    bool init(block_allocator &allocator, u32 initial_capacity = 0);
    bool init(arena_allocator &allocator, u32 initial_capacity = 0);
    void deinit();

    pool_handle push(T item);
    T *get(pool_handle handle);
    const T *get(pool_handle handle) const;
};

template <typename T> static bool maybe_resize(pool<T> &p);

template <typename T> bool pool<T>::init(block_allocator &allocator, u32 initial_capacity) {
    if (initial_capacity == 0) return false;

    data = (T *)allocator.alloc(sizeof(T) * initial_capacity);
    if (!data) return false;
    this->allocator = {allocation_type::block, &allocator};

    size = 0;
    capacity = initial_capacity;
    return true;
}

template <typename T> bool pool<T>::init(arena_allocator &allocator, u32 initial_capacity) {
    if (initial_capacity == 0) return false;

    data = (T *)allocator.alloc(sizeof(T) * initial_capacity);
    if (!data) return false;
    this->allocator = {allocation_type::arena, &allocator};

    size = 0;
    capacity = initial_capacity;
    return true;
}

template <typename T> void pool<T>::deinit() {
    if (data) {
        allocator.dealloc(data);
        data = nullptr;
        allocator = {};
    }
    size = 0;
    capacity = 0;
}

template <typename T> pool_handle pool<T>::push(T item) {
    if (!maybe_resize(*this)) return pool_handle::invalid();
    pool_handle h = {size};
    data[size++] = item;
    return h;
}

template <typename T> T *pool<T>::get(pool_handle handle) {
    if (handle.index >= size) return nullptr;
    return &data[handle.index];
}

template <typename T> const T *pool<T>::get(pool_handle handle) const {
    if (handle.index >= size) return nullptr;
    return &data[handle.index];
}

template <typename T> static bool maybe_resize(pool<T> &p) {
    if (p.size < p.capacity) return true;

    u32 new_capacity = p.capacity * 2;
    void *new_data = p.allocator.realloc(p.data, sizeof(T) * new_capacity);
    if (!new_data) {
        error("Failed to resize pool");
        return false;
    }

    p.data = (T *)new_data;
    p.capacity = new_capacity;
    return true;
}
