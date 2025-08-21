#pragma once

#include "defines.hpp"

enum class allocation_type {
    os,
    block,
    arena,
    borrowed,
};

struct parent_allocator {
    allocation_type type = allocation_type::borrowed;
    void *parent = nullptr;

    void *alloc(u32 size);
    void *realloc(void *ptr, u32 size);
    void dealloc(void *ptr);
};

struct block_allocator;
struct arena_allocator;

struct block_allocator {
    parent_allocator parent = {};

    void *memory = nullptr;
    void *free_head = nullptr;
    u32 block_size = 0;
    u32 total_blocks = 0;
    u32 free_blocks = 0;
    f32 grow_factor = 2.0f;
    bool owns_memory : 1 = false;

    bool init_as_root(u32 block_size, u32 block_count, f32 grow_factor = 2.0f);
    bool init(block_allocator &parent, u32 block_size, u32 block_count, f32 grow_factor = 2.0f);
    bool init(arena_allocator &parent, u32 block_size, u32 block_count);
    bool init(void *borrowed_memory, usize memory_size, u32 block_size);
    void deinit();

    void *alloc(u32 size);
    void *realloc(void *ptr, u32 size);
    void dealloc(void *ptr);
    void stats() const;
};

struct arena_allocator {
    parent_allocator parent = {};
    char *memory = nullptr;
    usize size = 0;
    usize used = 0;

    bool init_as_root(u32 size);
    bool init(block_allocator &parent, u32 size);
    bool init(arena_allocator &parent, u32 size);
    bool init(void *borrowed_memory, usize memory_size);
    void deinit();

    void *alloc(u32 size);
    void *alloc_array(u32 size, u32 count);
    void reset();
    void stats() const;
};
