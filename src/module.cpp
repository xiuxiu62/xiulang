#include "module.hpp"
#include "allocators.hpp"
#include "ast.hpp"
#include "collections.hpp"
// #include "collections/pool.cpp"
#include <cstring>

// template struct pool<use_info>;
// template struct pool<module_info>;

static const char **create_full_path(arena_allocator &allocator, const char **parent_path, u32 parent_depth,
                                     const char *name, u32 name_len);
static bool paths_equal(const char **path1, u32 path1_len, const char **path2, u32 path2_len);
static bool string_equals(const char *str1, u32 len1, const char *str2, u32 len2);

bool module_registry_init(module_registry &registry, arena_allocator &allocator) {
    registry.allocator = &allocator;
    return registry.modules.init(allocator, 32);
}

void module_registry_deinit(module_registry &registry) {
    registry.modules.deinit();
    registry.allocator = nullptr;
}

u32 module_count(const module_registry &registry) {
    return registry.modules.size;
}

module_info *get_module(module_registry &registry, pool_handle handle) {
    return registry.modules.get(handle);
}

pool_handle register_module(module_registry &registry, ast_decl_module *module_ast, pool_handle parent) {
    const char **parent_path = nullptr;
    u32 parent_depth = 0;

    if (parent.is_valid()) {
        const module_info *parent_module = get_module(registry, parent);
        if (!parent_module) return pool_handle::invalid();

        parent_path = parent_module->full_path;
        parent_depth = parent_module->path_depth;
    }

    // Create full path for this module
    const char **full_path =
        create_full_path(*registry.allocator, parent_path, parent_depth, module_ast->name, module_ast->name_len);
    if (!full_path) return pool_handle::invalid();

    u32 full_path_len = parent_depth + 1;

    // Check for duplicate module
    if (find_module_by_path(registry, full_path, full_path_len) != pool_handle::invalid()) {
        return pool_handle::invalid();
    }

    // Copy module name to arena
    char *name_copy = (char *)registry.allocator->alloc(module_ast->name_len + 1);
    if (!name_copy) return pool_handle::invalid();
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
    new_module.uses.init(*registry.allocator, 8); // Initialize with capacity for 8 uses
    new_module.is_analyzed = false;
    new_module.parent = parent;

    return registry.modules.push(new_module);
}

pool_handle find_module_by_path(module_registry &registry, const char **path, u32 path_len) {
    if (!path || path_len == 0) {
        return pool_handle::invalid();
    }

    // Iterate through all modules in the registry
    for (u32 i = 0; i < registry.modules.size; i++) {
        pool_handle handle{i};
        const module_info *module = registry.modules.get(handle);

        if (module && paths_equal(module->full_path, module->path_depth, path, path_len)) {
            return handle;
        }
    }

    return pool_handle::invalid();
}

pool_handle find_nested_module(module_registry &registry, pool_handle parent, const char *name, u32 name_len) {
    // pool_handle find_nested_module(module_registry &registry, module_info *parent, const char *name, u32 name_len) {
    if (parent.is_invalid() || !name || name_len == 0) {
        return pool_handle::invalid();
    }

    // Iterate through all modules to find children of the parent
    for (u32 i = 0; i < registry.modules.size; i++) {
        pool_handle handle{i};
        const module_info *module = registry.modules.get(handle);

        if (!module) continue;

        // Check if this module is a direct child of the parent
        if (module->parent.is_valid() && module->parent == parent) {
            // Check if the name matches
            if (string_equals(module->name, module->name_len, name, name_len)) {
                return handle;
            }
        }
    }

    return pool_handle::invalid();
}

// Helper function implementations
static const char **create_full_path(arena_allocator &allocator, const char **parent_path, u32 parent_depth,
                                     const char *name, u32 name_len) {
    u32 new_depth = parent_depth + 1;
    const char **full_path = (const char **)allocator.alloc_array(sizeof(const char *), new_depth);
    if (!full_path) return nullptr;

    for (u32 i = 0; i < parent_depth; i++) {
        full_path[i] = parent_path[i];
    }

    char *name_copy = (char *)allocator.alloc(name_len + 1);
    if (!name_copy) return nullptr;
    strncpy(name_copy, name, name_len);
    name_copy[name_len] = '\0';

    full_path[parent_depth] = name_copy;
    return full_path;
}

static bool paths_equal(const char **path1, u32 path1_len, const char **path2, u32 path2_len) {
    if (path1_len != path2_len) {
        return false;
    }

    for (u32 i = 0; i < path1_len; i++) {
        if (!path1[i] || !path2[i]) {
            return false;
        }
        if (strcmp(path1[i], path2[i]) != 0) {
            return false;
        }
    }

    return true;
}

static bool string_equals(const char *str1, u32 len1, const char *str2, u32 len2) {
    if (len1 != len2) {
        return false;
    }
    return strncmp(str1, str2, len1) == 0;
}
