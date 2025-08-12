#pragma once

#include "defines.hpp"

struct arena {
    char *memory = nullptr;
    usize size = 0;
    usize used = 0;
};

bool arena_init(struct arena &arena, u32 size);
void arena_deinit(struct arena &arena);

void *arena_alloc(struct arena &arena, u32 size);
void *arena_alloc_array(struct arena &arena, u32 size, u32 count);
void arena_reset(struct arena &arena);
void arena_stats(const struct arena &arena);
