#pragma once

#include "collections.hpp"
#include "defines.hpp"
#include "memory.hpp"

struct symbol;
struct symbol_table;
struct ast_decl_module;
struct module_info;

struct use_info {
    const char *local_name;
    u32 local_name_len;
    const char **source_path;

    pool_handle target_module;
    symbol *target_symbol;
    bool is_module_import;
};

struct module_info {
    const char *name;
    u32 name_len;
    const char **full_path;
    u32 path_depth;

    symbol_table *symbols;
    ast_decl_module *ast_node;

    pool<use_info> uses;

    bool is_analyzed;

    pool_handle parent;
};

// struct use_info {
//     symbol_table *symbols;
//     symbol *target_symbol;
//     bool is_fully_imported;
// };

struct use_resolution {
    struct resolved_item {
        const char *local_name;
        u32 local_name_len;
        const char **source_path;
        u32 source_path_len;
        symbol *resolved_symbol;
        ast_decl_module *source_module;
    };

    resolved_item *items;
    u32 item_count;
    u32 capacity;
};

struct module_registry {
    pool<module_info> modules;
    arena *loose_memory;
};

bool module_registry_init(module_registry &registry, arena &memory);
void module_registry_deinit(module_registry &registry);
pool_handle register_module(module_registry &registry, ast_decl_module *module_ast, pool_handle parent);
module_info *find_module_by_path(module_registry &registry, const char **path, u32 path_len);
module_info *find_nested_module(module_registry &registry, module_info *parent, const char *name, u32 name_len);
