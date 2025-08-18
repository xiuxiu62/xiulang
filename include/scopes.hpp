#pragma once

#include "memory.hpp"

struct symbol_table;

struct scope_manager {
    arena *allocator;
    symbol_table *global_scope = nullptr;
    symbol_table *current_scope = nullptr;
    u32 scope_counter;
    i32 current_stack_offset;
};

void push_scope(scope_manager &manager);
void pop_scope(scope_manager &manager);
