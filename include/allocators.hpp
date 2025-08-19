#pragma once

#include "defines.hpp"

struct block_allocator {
    bool init(u32 size);
    void deinit();

    void *alloc(u32 size);
    void *realloc(void *ptr, u32 size);
    void dealloc(void *ptr);
    void stats();
};

struct arena_allocator {
    block_allocator *parent_allocator;
    char *memory = nullptr;
    usize size = 0;
    usize used = 0;

    bool init(u32 size);
    void deinit();

    void *alloc(u32 size);
    void *alloc_array(u32 size, u32 count);
    void reset();
    void stats() const;
};
