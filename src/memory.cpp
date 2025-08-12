#include "memory.hpp"
#include "defines.hpp"
#include "logger.hpp"
#include <cstdlib>
#include <cstring>

#define ARENA_ALIGNMENT 8

static u32 align_size(u32 size, u32 alignment);

bool arena_init(arena &a, u32 size) {
    if (a.memory) {
        warn("Arena already initialized");
        return false;
    }

    auto memory = calloc(size, 1);
    if (!memory) {
        error("Failed to allocate %u bytes for arena", size);
        a.size = 0;
        return false;
    }

    a.memory = (char *)memory;
    a.size = size;
    a.used = 0;

    return true;
}

void arena_deinit(arena &a) {
    if (a.memory) {
        free(a.memory);
        a.memory = nullptr;
    }
    a.size = 0;
    a.used = 0;
}

void *arena_alloc(arena &a, u32 size) {
    if (!a.memory) {
        error("Arena not initialized");
        return nullptr;
    }

    if (size == 0) {
        warn("Attempted to allocate 0 bytes from arena");
        return nullptr;
    }

    u32 aligned_size = align_size(size, ARENA_ALIGNMENT);

    if (a.used + aligned_size > a.size) {
        error("Arena out of memory: requested %u bytes (aligned to %u), but only %lu bytes available", size,
              aligned_size, a.size - a.used);
        return nullptr;
    }

    void *ptr = a.memory + a.used;
    a.used += aligned_size;
    return ptr;
}

void *arena_alloc_array(arena &a, u32 size, u32 count) {
    if (count == 0) {
        warn("Attempted to allocate array with 0 elements");
        return nullptr;
    }

    if (count > u32_max / size) {
        error("Array allocation overflow: %u elements of %u bytes each", count, size);
        return nullptr;
    }

    return arena_alloc(a, size * count);
}

void arena_reset(arena &a) {
    if (!a.memory) {
        warn("Attempted to reset uninitialized arena");
        return;
    }

    a.used = 0;
    memset(a.memory, 0, a.size);
}

void arena_stats(const arena &a) {
    if (!a.memory) {
        info("Arena not initialized");
        return;
    }

    f64 usage_percentage = (f64)a.used / a.size * 100.0;
    info("Arena stats: %lu/%lu bytes used (%.1f%%)", a.used, a.size, usage_percentage);
}

static u32 align_size(u32 size, u32 alignment) {
    return (size + alignment - 1) & ~(alignment - 1);
}
