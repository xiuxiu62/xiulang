#pragma once

#include "defines.hpp"

struct ast_node;
struct ast_parameter;

enum class symbol_type : u32 {
    VARIABLE,
    PROCEDURE,
    PARAMETER,
    STRUCT,
    ENUM,
    UNION,
};

struct symbol {
    const char *name;
    u32 name_len;
    symbol_type type;
    ast_node *type_node;   // The type of this symbol
    ast_node *declaration; // The declaration node that created this symbol

    union {
        struct {
            i32 stack_offset;
            bool is_initialized;
            bool is_global;
        } variable;
        struct {
            u32 parameter_count;
            ast_parameter *parameters;
            ast_node *return_type;
            bool has_body; // true if procedure has implementation
        } procedure;
    } info;
};

struct symbol_table {
    symbol *symbols = nullptr;
    u32 count = 0;
    u32 capacity = 0;

    symbol_table *parent = nullptr;
    u32 scope_depth = 0;
};
