#include "allocators.hpp"
#include "defines.hpp"
#include "logger.hpp"
#include <cstdlib>
#include <cstring>

#define ARENA_ALIGNMENT 8

static u32 align_size(u32 size, u32 alignment);

bool arena_allocator::init(u32 size) {
    if (memory) {
        warn("Arena already initialized");
        return false;
    }

    void *memory = calloc(size, 1);
    if (!memory) {
        error("Failed to allocate %u bytes for arena", size);
        this->size = 0;
        return false;
    }

    memory = (char *)memory;
    this->size = size;
    used = 0;

    return true;
}

void arena_allocator::deinit() {
    if (memory) {
        free(memory);
        memory = nullptr;
    }
    size = 0;
    used = 0;
}

void *arena_allocator::alloc(u32 size) {
    if (!memory) {
        error("Arena not initialized");
        return nullptr;
    }

    if (size == 0) {
        warn("Attempted to allocate 0 bytes from arena");
        return nullptr;
    }

    u32 aligned_size = align_size(size, ARENA_ALIGNMENT);

    if (used + aligned_size > this->size) {
        error("Arena out of memory: requested %u bytes (aligned to %u), but only %lu bytes available", size,
              aligned_size, this->size - used);
        return nullptr;
    }

    void *ptr = memory + used;
    used += aligned_size;
    return ptr;
}

void *arena_allocator::alloc_array(u32 size, u32 count) {
    if (count == 0) {
        warn("Attempted to allocate array with 0 elements");
        return nullptr;
    }

    if (count > u32_max / size) {
        error("Array allocation overflow: %u elements of %u bytes each", count, size);
        return nullptr;
    }

    return alloc(size * count);
}

void arena_allocator::reset() {
    if (!memory) {
        warn("Attempted to reset uninitialized arena");
        return;
    }

    used = 0;
    memset(memory, 0, size);
}

void arena_allocator::stats() const {
    if (!memory) {
        info("Arena not initialized");
        return;
    }

    f64 usage_percentage = (f64)used / size * 100.0;
    info("Arena stats: %lu/%lu bytes used (%.1f%%)", used, size, usage_percentage);
}

static u32 align_size(u32 size, u32 alignment) {
    return (size + alignment - 1) & ~(alignment - 1);
}
