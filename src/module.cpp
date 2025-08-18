#include "module.hpp"
#include "ast.hpp"
#include "collections.hpp"
#include "memory.hpp"
#include <cstring>

static const char **create_full_path(arena &memory, const char **parent_path, u32 parent_depth, const char *name,
                                     u32 name_len);

bool module_registry_init(module_registry &registry, arena &memory) {
    registry.loose_memory = &memory;
    return registry.modules.init(32);
}

void module_registry_deinit(module_registry &registry) {
    registry.modules.deinit();
    registry.loose_memory = nullptr;
}

pool_handle register_module(module_registry &registry, ast_decl_module *module_ast, pool_handle parent) {
    const char **parent_path = nullptr;
    u32 parent_depth = 0;

    if (parent.is_valid()) {
        const module_info *parent = get_module(registry, parent_handle);
        if (!parent) return pool_handle::invalid();

        parent_path = parent->full_path;
        parent_depth = parent->path_depth;
    }

    // Create full path for this module
    const char **full_path =
        create_full_path(*registry.memory, parent_path, parent_depth, module_ast->name, module_ast->name_len);
    if (!full_path) return module_info::INVALID_HANDLE;

    u32 full_path_len = parent_depth + 1;

    // Check for duplicate module
    if (find_module_by_path(registry, full_path, full_path_len) != module_info::INVALID_HANDLE) {
        return module_info::INVALID_HANDLE; // Module already exists
    }

    // Copy module name to arena
    char *name_copy = (char *)arena_alloc(*registry.memory, module_ast->name_len + 1);
    if (!name_copy) return module_info::INVALID_HANDLE;
    strncpy(name_copy, module_ast->name, module_ast->name_len);
    name_copy[module_ast->name_len] = '\0';

    // Create new module info
    module_info new_module = {};
    new_module.name = name_copy;
    new_module.name_len = module_ast->name_len;
    new_module.full_path = full_path;
    new_module.path_depth = full_path_len;
    new_module.symbols = nullptr; // Will be created during analysis
    new_module.ast_node = module_ast;
    new_module.uses.init(8); // Initialize with capacity for 8 uses
    new_module.is_analyzed = false;
    new_module.parent_module = parent_handle;

    return registry.modules.push(new_module);
}

module_info *find_module_by_path(module_registry &registry, const char **path, u32 path_len) {
}

module_info *find_nested_module(module_registry &registry, module_info *parent, const char *name, u32 name_len) {
}

// bool register_module(module_registry &registery, ast_decl_module *module_node, const char **parent_path,
//                      u32 parent_depth) {
// }

static const char **create_full_path(arena &memory, const char **parent_path, u32 parent_depth, const char *name,
                                     u32 name_len) {
    u32 new_depth = parent_depth + 1;
    const char **full_path = (const char **)arena_alloc_array(memory, sizeof(const char *), new_depth);
    if (!full_path) return nullptr;

    for (u32 i = 0; i < parent_depth; i++) {
        full_path[i] = parent_path[i];
    }

    char *name_copy = (char *)arena_alloc(memory, name_len + 1);
    if (!name_copy) return nullptr;
    strncpy(name_copy, name, name_len);
    name_copy[name_len] = '\0';

    full_path[parent_depth] = name_copy;
    return full_path;
}
