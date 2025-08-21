#include "allocators.hpp"
#include "defines.hpp"
#include "logger.hpp"
#include <cstdlib>
#include <cstring>

#define ARENA_ALIGNMENT 8

static u32 align_size(u32 size, u32 alignment);
static bool maybe_resize(arena_allocator &a, u32 required_size);

bool arena_allocator::init_as_root(u32 size) {
    if (memory) {
        warn("Arena already initialized");
        return false;
    }

    void *new_memory = calloc(size, 1);
    if (!new_memory) {
        error("Failed to allocate %u bytes for arena", size);
        return false;
    }

    this->parent = {allocation_type::os, nullptr};
    this->memory = (char *)new_memory;
    this->size = size;
    this->used = 0;

    return true;
}

bool arena_allocator::init(block_allocator &parent, u32 size) {
    if (size == 0) return false;
    if (memory) {
        warn("Arena already initialized");
        return false;
    }

    void *new_memory = parent.alloc(size);
    if (!new_memory) {
        error("Failed to allocate %u bytes from parent block allocator", size);
        return false;
    }

    this->parent = {allocation_type::block, &parent};
    this->memory = (char *)new_memory;
    this->size = size;
    this->used = 0;

    // Clear the memory
    memset(this->memory, 0, size);

    return true;
}

bool arena_allocator::init(arena_allocator &parent, u32 size) {
    if (size == 0) return false;
    if (memory) {
        warn("Arena already initialized");
        return false;
    }

    void *new_memory = parent.alloc(size);
    if (!new_memory) {
        error("Failed to allocate %u bytes from parent arena allocator", size);
        return false;
    }

    this->parent = {allocation_type::arena, &parent};
    this->memory = (char *)new_memory;
    this->size = size;
    this->used = 0;

    // Memory is already zeroed from parent arena
    return true;
}

bool arena_allocator::init(void *borrowed_memory, usize memory_size) {
    if (!borrowed_memory || memory_size == 0) return false;
    if (memory) {
        warn("Arena already initialized");
        return false;
    }

    this->parent = {allocation_type::borrowed, nullptr};
    this->memory = (char *)borrowed_memory;
    this->size = memory_size;
    this->used = 0;

    return true;
}

void arena_allocator::deinit() {
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
            // Arena allocators don't support individual deallocation
            // Memory will be freed when parent arena is reset or destroyed
            break;
        case allocation_type::borrowed:
            // Don't free borrowed memory
            break;
        }
        memory = nullptr;
    }

    size = 0;
    used = 0;
    parent = {};
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

    // Check if we can resize when out of space
    if (used + aligned_size > this->size) {
        bool can_resize = (parent.type == allocation_type::os || parent.type == allocation_type::block);

        if (!can_resize || !maybe_resize(*this, aligned_size)) {
            error("Arena out of memory: requested %u bytes (aligned to %u), but only %lu bytes available", size,
                  aligned_size, this->size - used);
            return nullptr;
        }
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

static bool maybe_resize(arena_allocator &a, u32 required_size) {
    // Only OS and block parents can resize
    if (a.parent.type == allocation_type::arena || a.parent.type == allocation_type::borrowed) {
        return false;
    }

    // Calculate new size (grow by 2x or enough to fit the required allocation)
    usize new_size = a.size * 2;
    usize min_required = a.used + required_size;
    if (new_size < min_required) {
        new_size = min_required;
    }

    void *new_memory = a.parent.realloc(a.memory, new_size);
    if (!new_memory) return false;

    // Clear the new portion of memory
    char *new_portion = (char *)new_memory + a.size;
    memset(new_portion, 0, new_size - a.size);

    a.memory = (char *)new_memory;
    a.size = new_size;

    return true;
}
