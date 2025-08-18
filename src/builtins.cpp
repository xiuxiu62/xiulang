#include "builtins.hpp"
#include "ast.hpp"
#include "memory.hpp"
#include <cstring>

bool builtin_registry_init(builtin_registry &registry) {
    return registry.builtins.init(32); // Increased capacity for more builtins
}

void builtin_registry_deinit(builtin_registry &registry) {
    registry.builtins.deinit();
}

pool_handle register_builtin(builtin_registry &registry, builtin_procedure builtin) {
    return registry.builtins.push(builtin);
}

builtin_procedure *get_builtin(builtin_registry &registry, pool_handle handle) {
    return registry.builtins.get(handle);
}

builtin_procedure *find_builtin_procedure_by_name(builtin_registry &registry, const char *name, u32 name_len) {
    for (u32 i = 0; i < registry.builtins.size; i++) {
        pool_handle handle{i};
        builtin_procedure *builtin = registry.builtins.get(handle);
        if (builtin && strlen(builtin->name) == name_len && strncmp(builtin->name, name, name_len) == 0) {
            return builtin;
        }
    }
    return nullptr;
}

bool is_builtin_procedure_call(builtin_registry &registry, const char *name, u32 name_len) {
    return find_builtin_procedure_by_name(registry, name, name_len) != nullptr;
}

ast_node *create_builtin_type(arena &memory, builtin_type type) {
    ast_type_builtin *builtin = (ast_type_builtin *)arena_alloc(memory, sizeof(ast_type_builtin));
    if (!builtin) return nullptr;

    builtin->root = {.type = ast_node_type::TYPE_BUILTIN, .row = 0, .column = 0};
    builtin->type = type;

    return (ast_node *)builtin;
}

// Helper function to create parameter arrays
static ast_node **create_param_array(arena &memory, u32 count) {
    if (count == 0) return nullptr;
    return (ast_node **)arena_alloc_array(memory, sizeof(ast_node *), count);
}

void register_default_builtins(builtin_registry &registry, arena &memory) {
    // Create common types
    ast_node *void_type = create_builtin_type(memory, builtin_type::VOID);
    ast_node *bool_type = create_builtin_type(memory, builtin_type::BOOL);
    ast_node *char_type = create_builtin_type(memory, builtin_type::CHAR);
    ast_node *str_type = create_builtin_type(memory, builtin_type::STR);
    ast_node *u8_type = create_builtin_type(memory, builtin_type::U8);
    ast_node *u32_type = create_builtin_type(memory, builtin_type::U32);
    ast_node *u64_type = create_builtin_type(memory, builtin_type::U64);
    ast_node *usize_type = create_builtin_type(memory, builtin_type::USIZE);
    ast_node *s32_type = create_builtin_type(memory, builtin_type::S32);

    // =================================================================
    // COMPILER INTRINSICS - Always stay as builtins
    // =================================================================

    // sizeof<T>() -> usize
    builtin_procedure sizeof_proc = {
        .name = "sizeof",
        .type = builtin_procedure_type::size_of,
        .category = builtin_category::intrinsic,
        .param_count = 0, // Generic parameter, handled specially
        .param_types = nullptr,
        .return_type = usize_type,
        .requires_const_eval = true,
        .has_side_effects = false,
        .future_module = nullptr,
    };
    register_builtin(registry, sizeof_proc);

    // alignof<T>() -> usize
    builtin_procedure alignof_proc = {
        .name = "alignof",
        .type = builtin_procedure_type::align_of,
        .category = builtin_category::intrinsic,
        .param_count = 0,
        .param_types = nullptr,
        .return_type = usize_type,
        .requires_const_eval = true,
        .has_side_effects = false,
        .future_module = nullptr,
    };
    register_builtin(registry, alignof_proc);

    // compile_error(message: str) -> never
    ast_node **compile_error_params = create_param_array(memory, 1);
    if (compile_error_params) {
        compile_error_params[0] = str_type;
    }
    builtin_procedure compile_error_proc = {
        .name = "compile_error",
        .type = builtin_procedure_type::compile_error,
        .category = builtin_category::intrinsic,
        .param_count = 1,
        .param_types = compile_error_params,
        .return_type = void_type, // Never returns, but void for now
        .requires_const_eval = true,
        .has_side_effects = true,
        .future_module = nullptr,
    };
    register_builtin(registry, compile_error_proc);

    // =================================================================
    // CORE RUNTIME - Basic runtime functions
    // =================================================================

    // alloc(size: usize) -> *u8
    ast_node **alloc_params = create_param_array(memory, 1);
    if (alloc_params) {
        alloc_params[0] = usize_type;
    }
    // TODO: Create pointer type properly
    builtin_procedure alloc_proc = {
        .name = "alloc",
        .type = builtin_procedure_type::alloc,
        .category = builtin_category::core_runtime,
        .param_count = 1,
        .param_types = alloc_params,
        .return_type = u8_type, // Should be *u8, but simplified for now
        .requires_const_eval = false,
        .has_side_effects = true,
        .future_module = "core.mem",
    };
    register_builtin(registry, alloc_proc);

    // dealloc(ptr: *u8) -> void
    ast_node **dealloc_params = create_param_array(memory, 1);
    if (dealloc_params) {
        dealloc_params[0] = u8_type; // Should be *u8
    }
    builtin_procedure dealloc_proc = {
        .name = "dealloc",
        .type = builtin_procedure_type::dealloc,
        .category = builtin_category::core_runtime,
        .param_count = 1,
        .param_types = dealloc_params,
        .return_type = void_type,
        .requires_const_eval = false,
        .has_side_effects = true,
        .future_module = "core.mem",
    };
    register_builtin(registry, dealloc_proc);

    // panic(message: str) -> never
    ast_node **panic_params = create_param_array(memory, 1);
    if (panic_params) {
        panic_params[0] = str_type;
    }
    builtin_procedure panic_proc = {
        .name = "panic",
        .type = builtin_procedure_type::panic,
        .category = builtin_category::core_runtime,
        .param_count = 1,
        .param_types = panic_params,
        .return_type = void_type, // Never returns
        .requires_const_eval = false,
        .has_side_effects = true,
        .future_module = "core.panic",
    };
    register_builtin(registry, panic_proc);

    // assert(condition: bool, message: str) -> void
    ast_node **assert_params = create_param_array(memory, 2);
    if (assert_params) {
        assert_params[0] = bool_type;
        assert_params[1] = str_type;
    }
    builtin_procedure assert_proc = {
        .name = "assert",
        .type = builtin_procedure_type::assert,
        .category = builtin_category::core_runtime,
        .param_count = 2,
        .param_types = assert_params,
        .return_type = void_type,
        .requires_const_eval = false,
        .has_side_effects = true,
        .future_module = "core.panic",
    };
    register_builtin(registry, assert_proc);

    // =================================================================
    // FUTURE STDLIB - Will move to standard library eventually
    // =================================================================

    // print(message: str) -> void
    ast_node **print_params = create_param_array(memory, 1);
    if (print_params) {
        print_params[0] = str_type;
    }
    builtin_procedure print_proc = {
        .name = "print",
        .type = builtin_procedure_type::print,
        .category = builtin_category::stdlib_future,
        .param_count = 1,
        .param_types = print_params,
        .return_type = void_type,
        .requires_const_eval = false,
        .has_side_effects = true,
        .future_module = "std.io",
    };
    register_builtin(registry, print_proc);

    // println(message: str) -> void
    ast_node **println_params = create_param_array(memory, 1);
    if (println_params) {
        println_params[0] = str_type;
    }
    builtin_procedure println_proc = {
        .name = "println",
        .type = builtin_procedure_type::println,
        .category = builtin_category::stdlib_future,
        .param_count = 1,
        .param_types = println_params,
        .return_type = void_type,
        .requires_const_eval = false,
        .has_side_effects = true,
        .future_module = "std.io",
    };
    register_builtin(registry, println_proc);

    // file_open(path: str, mode: str) -> s32 (file handle)
    ast_node **file_open_params = create_param_array(memory, 2);
    if (file_open_params) {
        file_open_params[0] = str_type;
        file_open_params[1] = str_type;
    }
    builtin_procedure file_open_proc = {
        .name = "file_open",
        .type = builtin_procedure_type::file_open,
        .category = builtin_category::stdlib_future,
        .param_count = 2,
        .param_types = file_open_params,
        .return_type = s32_type,
        .requires_const_eval = false,
        .has_side_effects = true,
        .future_module = "std.fs",
    };
    register_builtin(registry, file_open_proc);

    // file_close(handle: s32) -> void
    ast_node **file_close_params = create_param_array(memory, 1);
    if (file_close_params) {
        file_close_params[0] = s32_type;
    }
    builtin_procedure file_close_proc = {
        .name = "file_close",
        .type = builtin_procedure_type::file_close,
        .category = builtin_category::stdlib_future,
        .param_count = 1,
        .param_types = file_close_params,
        .return_type = void_type,
        .requires_const_eval = false,
        .has_side_effects = true,
        .future_module = "std.fs",
    };
    register_builtin(registry, file_close_proc);

    // str_len(s: str) -> usize
    ast_node **str_len_params = create_param_array(memory, 1);
    if (str_len_params) {
        str_len_params[0] = str_type;
    }
    builtin_procedure str_len_proc = {
        .name = "len",
        .type = builtin_procedure_type::str_len,
        .category = builtin_category::stdlib_future,
        .param_count = 1,
        .param_types = str_len_params,
        .return_type = usize_type,
        .requires_const_eval = false,
        .has_side_effects = false,
        .future_module = "std.str",
    };
    register_builtin(registry, str_len_proc);
}

// Validation helper
bool validate_builtin_call(builtin_registry &registry, ast_expr_call *call) {
    // TODO: Implement call validation
    // Check parameter count, types, etc.
    return true;
}

ast_node *get_builtin_return_type(builtin_registry &registry, const builtin_procedure &builtin, ast_expr_call *call) {
    // Special handling for generic builtins like sizeof
    if (builtin.type == builtin_procedure_type::size_of || builtin.type == builtin_procedure_type::align_of) {
        // These return usize regardless of the type parameter
        return builtin.return_type;
    }

    return builtin.return_type;
}
