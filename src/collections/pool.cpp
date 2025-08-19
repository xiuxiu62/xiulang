#include "allocators.hpp"
#include "collections.hpp"
#include "logger.hpp"

template <typename T> static bool maybe_resize(pool<T> &p);

template <typename T> bool pool<T>::init(block_allocator &allocator, u32 initial_capacity) {
    if (initial_capacity == 0) return false;

    data = allocator.alloc(sizeof(T) * initial_capacity);
    if (!data) return false;
    this->allocator = allocator;

    size = 0;
    capacity = initial_capacity;
    return true;
}

template <typename T> void pool<T>::deinit() {
    if (data) {
        allocator->dealloc(data);
        data = nullptr;
        allocator = nullptr;
    }
    size = 0;
    capacity = 0;
}

template <typename T> pool_handle pool<T>::push(T item) {
    if (!maybe_resize(this)) return pool_handle::invalid();
    pool_handle h = {size};
    data[size++] = item;
    return h;
}

template <typename T> T *pool<T>::get(pool_handle handle) {
    if (handle.index >= size) return nullptr;
    return data[handle];
}

template <typename T> const T *pool<T>::get(pool_handle handle) const {
    if (handle.index >= size) return nullptr;
    return data[handle];
}

template <typename T> static bool maybe_resize(pool<T> &p) {
    if (p.size < p.capacity) return true;

    u32 new_capacity = p.capacity * 2;
    void *new_data = p.allocator->realloc(p.data, sizeof(T) * new_capacity);
    if (!new_data) {
        error("Failed to resize pool");
        return false;
    }

    return true;
}
