#pragma once

#include "allocators.hpp"
#include "ast.hpp"
#include "collections.hpp"

struct ast_node;
struct ast_expr_call;

enum class builtin_procedure_type : u32 {
    size_of,
    align_of,
    type_info,
    compile_error,

    alloc,
    realloc,
    dealloc,

    print,
    println,

    file_open,
    file_close,
    file_read,
    file_write,

    str_len,

    panic,
    assert,
};

enum class builtin_category : u32 {
    intrinsic,     // Must always be built into the compiler
    core_runtime,  // basic runtime (panic, malloc),
    stdlib_future, // Will eventually move to stdlib (print, file ops)
};

struct builtin_procedure {
    const char *name;
    builtin_procedure_type type;
    builtin_category category;
    u32 param_count;
    ast_node **param_types;
    ast_node *return_type;
    bool requires_const_eval; // Must be evaluated at compile time
    bool has_side_effects;
    const char *future_module; // For stdlib_future builtins
};

struct builtin_registry {
    pool<builtin_procedure> builtins;
};
// Registry management
bool builtin_registry_init(builtin_registry &registry);
void builtin_registry_deinit(builtin_registry &registry);
pool_handle register_builtin(builtin_registry &registry, builtin_procedure builtin);

// Lookup functions
builtin_procedure *get_builtin(builtin_registry &registry, pool_handle handle);
builtin_procedure *find_builtin_procedure_by_name(builtin_registry &registry, const char *name, u32 name_len);
bool is_builtin_procedure_call(builtin_registry &registry, const char *name, u32 name_len);

// Type creation
ast_node *create_builtin_type(arena_allocator &allocator, builtin_type type);

// Registration
void register_default_builtins(builtin_registry &registry, arena_allocator &allocator);

// Category helpers
constexpr bool is_intrinsic_builtin(const builtin_procedure &builtin) {
    return builtin.category == builtin_category::intrinsic;
}

constexpr bool is_core_runtime_builtin(const builtin_procedure &builtin) {
    return builtin.category == builtin_category::core_runtime;
}

constexpr bool will_move_to_stdlib(const builtin_procedure &builtin) {
    return builtin.category == builtin_category::stdlib_future;
}

// Analysis helpers
bool validate_builtin_call(builtin_registry &registry, ast_expr_call *call);
ast_node *get_builtin_return_type(builtin_registry &registry, const builtin_procedure &builtin, ast_expr_call *call);
