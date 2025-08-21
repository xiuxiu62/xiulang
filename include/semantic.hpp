// #pragma once

// #include "allocators.hpp"
// #include "ast.hpp"
// #include "builtins.hpp"
// #include "defines.hpp"
// #include "module.hpp"

// enum class symbol_type : u32 {
//     VARIABLE,
//     PROCEDURE,
//     PARAMETER,
//     STRUCT,
//     ENUM,
//     UNION,
// };

// struct symbol {
//     const char *name;
//     u32 name_len;
//     symbol_type type;
//     ast_node *type_node;   // The type of this symbol
//     ast_node *declaration; // The declaration node that created this symbol

//     union {
//         struct {
//             i32 stack_offset;
//             bool is_initialized;
//             bool is_global;
//         } variable;
//         struct {
//             u32 parameter_count;
//             ast_parameter *parameters;
//             ast_node *return_type;
//             bool has_body; // true if procedure has implementation
//         } procedure;
//     } info;
// };

// struct symbol_table {
//     symbol *symbols = nullptr;
//     u32 count = 0;
//     u32 capacity = 0;

//     symbol_table *parent = nullptr;
//     u32 scope_depth = 0;
// };

// struct semantic_analyzer {
//     arena_allocator *allocator;
//     symbol_table *current_scope;
//     symbol_table *global_scope;

//     bool has_error;
//     const char *error_message;
//     u32 error_row, error_column;

//     ast_decl_procedure *current_procedure;
//     ast_node *expected_return_type;

//     // i32 current_stack_offset;
//     // u32 scope_counter; // For generating unique scope IDs

//     builtin_procedure *builtin_procedures;
//     u32 builtin_procedure_count;

//     builtin_registry builtins;
//     module_registry modules;

//     ast_decl_module *current_module;
//     const char **current_module_path;
//     u32 current_path_depth;

//     use_resolution current_uses;
// };

// bool semantic_analyzer_init(semantic_analyzer &analyzer, arena_allocator &allocator);
// void semantic_analyzer_deinit(semantic_analyzer &analyzer);
// bool analyze(semantic_analyzer &analyzer, ast_node *program);

// ast_node *create_builtin_type(builtin_registry &registry, builtin_type type);
// // void register_builtin_procedures(semantic_analyzer &analyzer);
// // builtin_procedure *lookup_builtin_procedure(semantic_analyzer &analyzer, const char *name, u32 name_len);
// // bool is_builtin_procedure_call(semantic_analyzer &analyzer, const char *name, u32 name_len);
// bool analyze_call_expression(semantic_analyzer &analyzer, ast_expr_call *call);
// // ast_node *create_builtin_type(semantic_analyzer &analyzer, builtin_type type);

// // Symbol table management
// symbol_table *symbol_table_create(semantic_analyzer &analyzer, symbol_table *parent);
// void symbol_table_destroy(symbol_table *table);
// bool symbol_table_add(semantic_analyzer &analyzer, symbol_table *table, const symbol &sym);
// symbol *symbol_table_lookup(symbol_table *table, const char *name, u32 name_len);
// symbol *symbol_table_lookup_current_scope(symbol_table *table, const char *name, u32 name_len);

// // // Scope management
// // void push_scope(semantic_analyzer &analyzer);
// // void pop_scope(semantic_analyzer &analyzer);

// bool analyze_translation_unit(semantic_analyzer &analyzer, ast_translation_unit *unit);
// bool analyze_declaration(semantic_analyzer &analyzer, ast_node *decl);
// bool analyze_variable_declaration(semantic_analyzer &analyzer, ast_decl_variable *var_decl);
// bool analyze_procedure_declaration(semantic_analyzer &analyzer, ast_decl_procedure *proc_decl);

// bool analyze_statement(semantic_analyzer &analyzer, ast_node *stmt);
// bool analyze_compound_statement(semantic_analyzer &analyzer, ast_stmt_compound *compound);
// bool analyze_if_statement(semantic_analyzer &analyzer, ast_stmt_if *if_stmt);
// bool analyze_return_statement(semantic_analyzer &analyzer, ast_stmt_return *return_stmt);
// bool analyze_expression_statement(semantic_analyzer &analyzer, ast_stmt_expression *expr_stmt);

// bool analyze_expression(semantic_analyzer &analyzer, ast_node *expr);
// bool analyze_binary_expression(semantic_analyzer &analyzer, ast_expr_binary *binary);
// bool analyze_unary_expression(semantic_analyzer &analyzer, ast_expr_unary *unary);
// bool analyze_literal_expression(semantic_analyzer &analyzer, ast_expr_literal *literal);
// bool analyze_identifier_expression(semantic_analyzer &analyzer, ast_expr_identifier *identifier);

// // Type checking and inference
// ast_node *infer_expression_type(semantic_analyzer &analyzer, ast_node *expr);
// bool types_compatible(ast_node *type1, ast_node *type2);
// bool is_numeric_type(ast_node *type);
// bool is_integer_type(ast_node *type);
// bool is_float_type(ast_node *type);
// ast_node *get_common_type(semantic_analyzer &analyzer, ast_node *type1, ast_node *type2);

// // Error reporting
// void semantic_error(semantic_analyzer &analyzer, const char *message, u32 row, u32 col);
// void semantic_error_at_node(semantic_analyzer &analyzer, const char *message, ast_node *node);

// // =============================================================================
// // MODULE SYSTEM
// // =============================================================================

// // Module registration and lookup
// bool register_module(semantic_analyzer &analyzer, ast_decl_module *module, const char **parent_path, u32
// parent_depth); ast_decl_module *find_module_by_path(semantic_analyzer &analyzer, const char **path, u32 path_len);
// ast_decl_module *find_module_relative(semantic_analyzer &analyzer, const char **path, u32 path_len);

// // Use declaration processing
// bool process_use_declaration(semantic_analyzer &analyzer, ast_decl_use *use_decl);
// bool resolve_use_item(semantic_analyzer &analyzer, ast_decl_use::use_item *item);

// // Symbol lookup with module resolution
// symbol *lookup_in_current_scope(semantic_analyzer &analyzer, const char *name, u32 name_len);
// symbol *lookup_with_uses(semantic_analyzer &analyzer, const char *name, u32 name_len);
// symbol *lookup_in_module(semantic_analyzer &analyzer, ast_decl_module *module, const char *name, u32 name_len);

// // Member access analysis
// bool analyze_member_access_expr(semantic_analyzer &analyzer, ast_expr_member_access *expr);
// ast_decl_module *resolve_module_path(semantic_analyzer &analyzer, const char **path, u32 path_len);

// // Module hierarchy traversal
// bool analyze_module_declaration(semantic_analyzer &analyzer, ast_decl_module *module);
// void enter_module_scope(semantic_analyzer &analyzer, ast_decl_module *module);
// void exit_module_scope(semantic_analyzer &analyzer);

// // Utility
// bool symbol_names_equal(const char *name1, u32 len1, const char *name2, u32 len2);
// const char *symbol_type_to_string(symbol_type type);

// const char **append_to_path(semantic_analyzer &analyzer, const char **base_path, u32 base_len, const char *name,
//                             u32 name_len);
// const char **copy_path(semantic_analyzer &analyzer, const char **path, u32 path_len);
// bool paths_equal(const char **path1, u32 len1, const char **path2, u32 len2);
// char *path_to_string(const char **path, u32 path_len); // For error messages

// // Module hierarchy
// bool is_ancestor_module(const char **ancestor_path, u32 ancestor_len, const char **descendant_path, u32
// descendant_len); const char **get_relative_path(const char **from_path, u32 from_len, const char **to_path, u32
// to_len);

// // Scope management
// void push_module_scope(semantic_analyzer &analyzer, ast_decl_module *module);
// void pop_module_scope(semantic_analyzer &analyzer);
