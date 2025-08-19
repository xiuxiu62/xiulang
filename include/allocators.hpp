#pragma once

#include "defines.hpp"

struct block_allocator {
    struct growth_strategy {
        bool auto_resizes = false;
        f32 growth_factor = 2.0f;
    };

    void *memory = nullptr;
    void *free_head = nullptr;
    u32 block_size = 0;
    u32 total_blocks = 0;
    u32 free_blocks = 0;
    growth_strategy grow_strat;
    bool owns_memory : 1 = false;

    bool init(u32 block_size, u32 block_count, growth_strategy grow_strat = {false, 2.0f});
    bool init(void *borrowed_memory, usize memory_size, u32 block_size);
    void deinit();

    void *alloc(u32 size);
    void *realloc(void *ptr, u32 size);
    void dealloc(void *ptr);
    void stats() const;
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
