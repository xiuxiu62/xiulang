#include "allocators.hpp"
#include "defines.hpp"
#include "logger.hpp"
#include <cstdlib>
#include <cstring>

static void init_freelist(block_allocator &a);
static bool maybe_resize(block_allocator &a);
static bool is_valid_ptr(block_allocator &a, void *ptr);

bool block_allocator::init_as_root(u32 block_size, u32 block_count, f32 grow_factor) {
    if (block_size < sizeof(void *)) {
        block_size = sizeof(void *);
    }

    usize total_size = block_size * block_count;
    this->memory = malloc(total_size);
    if (!this->memory) return false;

    this->parent = {allocation_type::os, nullptr};
    this->free_head = (char *)memory;
    this->block_size = block_size;
    this->total_blocks = block_count;
    this->free_blocks = block_count;
    this->grow_factor = grow_factor;

    init_freelist(*this);
    return true;
}

bool block_allocator::init(block_allocator &parent, u32 block_size, u32 block_count, f32 grow_factor) {
    if (block_count == 0) return false;

    if (block_size < sizeof(void *)) {
        block_size = sizeof(void *);
    }

    usize total_size = block_size * block_count;
    this->memory = parent.alloc(total_size);
    if (!this->memory) return false;

    this->parent = {allocation_type::block, &parent};
    this->free_head = (char *)memory;
    this->block_size = block_size;
    this->total_blocks = block_count;
    this->free_blocks = block_count;
    this->grow_factor = grow_factor;

    init_freelist(*this);
    return true;
}

bool block_allocator::init(arena_allocator &parent, u32 block_size, u32 block_count) {
    if (block_count == 0) return false;

    if (block_size < sizeof(void *)) {
        block_size = sizeof(void *);
    }

    usize total_size = block_size * block_count;
    this->memory = parent.alloc(total_size);
    if (!this->memory) return false;

    this->parent = {allocation_type::arena, &parent};
    this->free_head = (char *)memory;
    this->block_size = block_size;
    this->total_blocks = block_count;
    this->free_blocks = block_count;
    this->grow_factor = 2.0f; // Won't be used since arena children can't resize

    init_freelist(*this);
    return true;
}

bool block_allocator::init(void *borrowed_memory, usize memory_size, u32 block_size) {
    if (!borrowed_memory || memory_size == 0 || block_size == 0) {
        return false;
    }

    if (block_size < sizeof(void *)) {
        block_size = sizeof(void *);
    }

    u32 block_count = memory_size / block_size;
    if (block_count == 0) return false; // Not enough memory

    this->parent = {allocation_type::borrowed, nullptr};
    this->memory = borrowed_memory;
    this->free_head = (char *)borrowed_memory;
    this->block_size = block_size;
    this->total_blocks = block_count;
    this->free_blocks = block_count;
    this->grow_factor = 1.0f; // Won't be used since memory's borrowed

    init_freelist(*this);
    return true;
}

void block_allocator::deinit() {
    if (memory) {
        switch (parent.type) {
        case allocation_type::os:
            free(memory);
            break;
        case allocation_type::block: {
            block_allocator *parent_alloc = (block_allocator *)parent.parent;
            parent_alloc->dealloc(memory);
            break;
        }
        case allocation_type::arena:
            break;
        case allocation_type::borrowed:
            // Don't free borrowed memory
            break;
        }
        memory = nullptr;
    }

    free_head = nullptr;
    block_size = 0;
    total_blocks = 0;
    free_blocks = 0;
    parent = {};
}

void *block_allocator::alloc(u32 size) {
    // Check if we can resize when out of blocks
    bool can_resize = (parent.type == allocation_type::os || parent.type == allocation_type::block);

    // Ensure capacity
    if (!free_head && (!can_resize || !maybe_resize(*this))) {
        return nullptr;
    }

    void *block = free_head;     // Grab block
    free_head = *(void **)block; // Update head
    free_blocks--;
    return block;
}

void *block_allocator::realloc(void *ptr, u32 size) {
    if (!ptr) return alloc(size);
    if (size == 0) {
        dealloc(ptr);
        return nullptr;
    }
    if (!is_valid_ptr(*this, ptr)) return nullptr;
    if (size <= block_size) return ptr;

    void *new_ptr = alloc(size);
    if (!new_ptr) return nullptr;

    u32 copy_size = (size < block_size) ? size : block_size;
    memcpy(new_ptr, ptr, copy_size);
    dealloc(ptr);
    return new_ptr;
}

void block_allocator::dealloc(void *ptr) {
    if (!ptr) return;

    *(void **)ptr = free_head;
    free_head = ptr;
    free_blocks++;
}

void block_allocator::stats() const {
    if (!memory) {
        info("Block allocator not initialized");
        return;
    }

    u32 used_blocks = total_blocks - free_blocks;
    f64 usage_percentage = (f64)used_blocks / total_blocks * 100.0;
    info("Block allocator: %u/%u blocks (%dkb) used (%.1f%%)", used_blocks, total_blocks, block_size / kilobytes(1),
         usage_percentage);
}

static void init_freelist(block_allocator &a) {
    char *current = (char *)a.memory;
    for (u32 i = 0; i < a.total_blocks - 1; i++) {
        void **next_ptr = (void **)current;
        current += a.block_size;
        *next_ptr = current;
    }

    // Last block points to nullptr
    void **last_ptr = (void **)(current);
    *last_ptr = nullptr;
}

static bool maybe_resize(block_allocator &a) {
    // Only OS and block parents can resize
    if (a.parent.type == allocation_type::arena || a.parent.type == allocation_type::borrowed) {
        return false;
    }

    u32 old_block_count = a.total_blocks;
    u32 new_block_count = a.total_blocks * a.grow_factor;
    usize old_total_size = old_block_count * a.block_size;
    usize new_total_size = new_block_count * a.block_size;

    void *new_memory = a.parent.realloc(a.memory, new_total_size);
    if (!new_memory) return false;

    u32 added_blocks = new_block_count - old_block_count;

    a.memory = new_memory;
    a.total_blocks = new_block_count;
    a.free_blocks += added_blocks;

    // Initialize the new blocks starting after the old blocks
    char *new_block_start = (char *)new_memory + old_total_size;
    char *current = new_block_start;

    for (u32 i = 0; i < added_blocks - 1; i++) {
        void **next_ptr = (void **)current;
        current += a.block_size;
        *next_ptr = current;
    }

    // Last new block points to old free_head
    void **last_new_ptr = (void **)current;
    *last_new_ptr = a.free_head;
    a.free_head = new_block_start;

    return true;
}

static bool is_valid_ptr(block_allocator &a, void *ptr) {
    char *p = (char *)ptr;
    char *base = (char *)a.memory;

    if (p < base || p >= base + (a.total_blocks * a.block_size)) {
        return false;
    }

    return ((p - base) % a.block_size) == 0;
}
