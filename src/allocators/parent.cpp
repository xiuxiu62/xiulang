#include "allocators.hpp"
#include <cstdlib>

void *parent_allocator::alloc(u32 size) {
    if (!parent) return nullptr;
    switch (type) {
    case allocation_type::os:
        return ::malloc(size);
    case allocation_type::block:
        return ((block_allocator *)parent)->alloc(size);
    case allocation_type::arena:
        return nullptr;
    case allocation_type::borrowed:
        return nullptr;
    }
}

void *parent_allocator::realloc(void *ptr, u32 size) {
    if (!parent) return nullptr;
    switch (type) {
    case allocation_type::os:
        return ::realloc(ptr, size);
    case allocation_type::block:
        return ((block_allocator *)parent)->realloc(ptr, size);
    case allocation_type::arena:
        return nullptr;
    case allocation_type::borrowed:
        return nullptr;
    }
}

void parent_allocator::dealloc(void *ptr) {
    if (!parent) return;
    switch (type) {
    case allocation_type::os:
        ::free(ptr);
    case allocation_type::block:
        ((block_allocator *)parent)->dealloc(ptr);
    case allocation_type::arena:
    case allocation_type::borrowed:
        return;
    }
}
