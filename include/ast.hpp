#pragma once

#include "defines.hpp"
#include "token.hpp"

enum class ast_node_type : u32 {
    EXPR_LITERAL = 0,
    EXPR_IDENTIFIER = 1,
    EXPR_BINARY = 2,
    EXPR_UNARY = 3,
    EXPR_CALL = 4,
    EXPR_MEMBER = 5,
    EXPR_INDEX = 6,
    EXPR_CAST = 7,
    EXPR_SIZEOF = 8,
    EXPR_MEMBER_ACCESS = 9,

    STMT_EXPRESSION = 30,
    STMT_COMPOUND = 31,
    STMT_IF = 32,
    STMT_WHILE = 33,
    STMT_FOR = 34,
    STMT_RETURN = 35,
    STMT_BREAK = 36,
    STMT_CONTINUE = 37,
    STMT_SWITCH = 38,
    STMT_CASE = 39,
    STMT_DEFAULT = 40,
    STMT_GOTO = 41,
    STMT_LABEL = 42,
    STMT_DEFER = 43,

    DECL_VARIABLE = 60,
    DECL_PROCEDURE = 61,
    DECL_STRUCT = 62,
    DECL_UNION = 63,
    DECL_ENUM = 64,
    DECL_TYPEDEF = 65,
    DECL_MODULE = 66,
    DECL_USE = 67,

    TYPE_BUILTIN = 90,
    TYPE_POINTER = 91,
    TYPE_ARRAY = 92,
    TYPE_PROCEDURE = 93,
    TYPE_STRUCT = 94,
    TYPE_UNION = 95,
    TYPE_ENUM = 96,
    TYPE_INFERRED = 97,

    TRANSLATION_UNIT = u32_max,
};

enum class literal_type {
    INTEGER,
    FLOAT,
    CHARACTER,
    STRING,
    BOOLEAN,
};

// Updated for new language types
enum class builtin_type {
    VOID,

    // Boolean
    BOOL,

    // Character and string types
    CHAR,
    STR, // string type (like const char* but builtin)

    // Unsigned integer types
    U8,
    U16,
    U32,
    U64,
    USIZE, // platform-dependent unsigned size

    // Signed integer types
    S8,
    S16,
    S32,
    S64,
    SSIZE, // platform-dependent signed size

    // Floating point types
    F32,
    F64, // explicit float types

};

enum class binary_op {
    // Arithmetic
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,

    // Comparison
    EQ,
    NE,
    LT,
    LE,
    GT,
    GE,

    // Logical
    LOGICAL_AND,
    LOGICAL_OR,

    // Bitwise
    BITWISE_AND,
    BITWISE_OR,
    BITWISE_XOR,
    LEFT_SHIFT,
    RIGHT_SHIFT,

    // Assignment
    ASSIGN,
    ADD_ASSIGN,
    SUB_ASSIGN,
    MUL_ASSIGN,
    DIV_ASSIGN,
    MOD_ASSIGN,
    AND_ASSIGN,
    OR_ASSIGN,
    XOR_ASSIGN,
    LEFT_SHIFT_ASSIGN,
    RIGHT_SHIFT_ASSIGN,
};

enum class unary_op {
    PLUS,
    MINUS,
    NOT,
    BITWISE_NOT,
    PRE_INC,
    POST_INC,
    PRE_DEC,
    POST_DEC,
    DEREFERENCE,
    ADDRESS_OF,
};

struct ast_node {
    ast_node_type type;
    u32 row, column;
};

// =============================================================================
// EXPRESSIONS
// =============================================================================

struct ast_expr_literal {
    ast_node root;
    literal_type type;
    const char *value;
    u32 value_len;
};

struct ast_expr_identifier {
    ast_node root;
    const char *name;
    u32 name_len;
};

struct ast_expr_binary {
    ast_node root;
    binary_op op;
    ast_node *left;
    ast_node *right;
};

struct ast_expr_unary {
    ast_node root;
    unary_op op;
    ast_node *operand;
};

struct ast_expr_call {
    ast_node root;
    ast_node *procedure;
    ast_node **arguments;
    u32 argument_count;
};

struct ast_expr_index {
    ast_node root;
    ast_node *array;
    ast_node *index;
};

struct ast_expr_cast {
    ast_node root;
    ast_node *target_type;
    ast_node *expression;
};

struct ast_expr_sizeof {
    ast_node root;
    ast_node *type;
    ast_node *expression;
};

struct ast_expr_member {
    ast_node root;
    ast_node *object;
    const char *member;
    u32 member_len;
};

struct ast_expr_member_access {
    ast_node root;
    ast_node *object;
    const char *member;
    u32 member_len;

    bool is_module_access;
    struct ast_decl_module *resolved_module;
};

// =============================================================================
// STATEMENTS
// =============================================================================

struct ast_stmt_expression {
    ast_node root;
    ast_node *expression;
};

struct ast_stmt_compound {
    ast_node root;
    ast_node **statements;
    u32 statement_count;
};

struct ast_stmt_if {
    ast_node root;
    ast_node *condition;
    ast_node *then_stmt;
    ast_node *else_stmt; // null when no else
};

struct ast_stmt_while {
    ast_node root;
    ast_node *condition;
    ast_node *body;
};

struct ast_stmt_for {
    ast_node root;
    ast_node *init;
    ast_node *condition;
    ast_node *update;
    ast_node *body;
};

struct ast_stmt_return {
    ast_node root;
    ast_node *value;
};

struct ast_stmt_break {
    ast_node root;
};

struct ast_stmt_continue {
    ast_node root;
};

struct ast_stmt_defer {
    ast_node root;
    ast_node *action;
};

struct ast_stmt_switch {
    ast_node root;
    ast_node *expression;
    ast_node *body;
};

struct ast_stmt_case {
    ast_node root;
    ast_node *value;
    ast_node *statement;
};

struct ast_stmt_default {
    ast_node root;
    ast_node *statement;
};

struct ast_stmt_goto {
    ast_node root;
    const char *label;
    u32 label_len;
};

struct ast_stmt_label {
    ast_node root;
    const char *label;
    u32 label_len;
    ast_node *statement;
};

// =============================================================================
// TYPES
// =============================================================================

struct ast_type_builtin {
    ast_node root;
    builtin_type type;
};

struct ast_type_pointer {
    ast_node root;
    ast_node *to;
};

struct ast_type_array {
    ast_node root;
    ast_node *element_type;
    ast_node *size;
};

struct ast_type_procedure {
    ast_node root;
    ast_node *return_type;
    ast_node **parameter_types;
    u32 parameter_count;
    bool is_variadic;
};

// NEW: Type inference placeholder
struct ast_type_inferred {
    ast_node root;
    // The actual type will be determined during semantic analysis
    ast_node *inferred_type; // Filled in by semantic analyzer, initially nullptr
};

struct ast_type_struct {
    ast_node root;
    const char *name;
    u32 name_len;
};

struct ast_type_union {
    ast_node root;
    const char *name;
    u32 name_len;
};

struct ast_type_enum {
    ast_node root;
    const char *name;
    u32 name_len;
};

// =============================================================================
// DECLARATIONS (Updated for new language)
// =============================================================================

struct ast_decl_variable {
    ast_node root;
    ast_node *var_type; // Can be nullptr for type inference (name :: value)
    const char *name;
    u32 name_len;
    ast_node *initializer;
    bool is_public;         // NEW: Track pub visibility
    bool has_explicit_type; // NEW: True for "name : type : value", false for "name :: value"
};

// Updated parameter structure for new syntax
struct ast_parameter {
    ast_node *param_type;
    const char *name;
    u32 name_len;
    bool has_default_value;  // NEW: For future default parameters
    ast_node *default_value; // NEW: For future default parameters
};

struct ast_decl_procedure {
    ast_node root;
    ast_node *return_type; // Can be nullptr if no -> specified
    const char *name;
    u32 name_len;
    ast_parameter *parameters;
    u32 parameter_count;
    ast_node *body;                // Can be nullptr for forward declarations
    bool is_public;                // NEW: Track pub visibility
    bool has_explicit_return_type; // NEW: True if -> type was specified
};

struct ast_decl_struct {
    ast_node root;
    const char *name;
    u32 name_len;
    ast_node **fields;
    u32 field_count;
};

struct ast_decl_union {
    ast_node root;
    const char *name;
    u32 name_len;
    ast_node **fields;
    u32 field_count;
};

struct ast_decl_enum {
    ast_node root;
    const char *name;
    u32 name_len;
    ast_node **values;
    u32 value_count;
};

struct ast_decl_typedef {
    ast_node root;
    const char *name;
    u32 name_len;
    ast_node *type;
};

struct ast_decl_module {
    ast_node root;
    const char *name;
    u32 name_len;
    ast_node **declarations;
    u32 declaration_count;

    struct ast_decl_module *parent_module;
    const char **full_path;
    u32 path_depth;
};

struct ast_decl_use {
    ast_node root;

    struct use_item {
        const char **path;
        u32 path_len;

        struct group_item {
            const char *name;
            u32 name_len;
            const char *alias;
            u32 alias_len;
        };

        group_item *group_items;
        u32 group_count;

        bool is_group;
        const char *alias;
        u32 alias_len;
    };

    use_item *items;
    u32 item_count;
};

// =============================================================================
// PROGRAM STRUCTURE
// =============================================================================

struct ast_translation_unit {
    ast_node root;
    ast_node **declarations;
    u32 declaration_count;
};

// Create an inferred type node
inline constexpr ast_node *create_inferred_type(u32 line, u32 column) {
    // This would need to be implemented with proper arena allocation
    // For now, just a declaration
    return nullptr;
}

// Check if a variable declaration uses type inference
inline constexpr bool is_type_inferred(ast_decl_variable *var_decl) {
    return var_decl->var_type == nullptr || var_decl->var_type->type == ast_node_type::TYPE_INFERRED;
}

// Check if a procedure has an explicit return type
inline constexpr bool has_explicit_return_type(ast_decl_procedure *proc_decl) {
    return proc_decl->has_explicit_return_type;
}

// Get the string representation of builtin types for the new language
inline constexpr const char *builtin_type_to_string(builtin_type type) {
    switch (type) {
    case builtin_type::VOID:
        return "void";
    case builtin_type::BOOL:
        return "bool";
    case builtin_type::CHAR:
        return "char";
    case builtin_type::STR:
        return "str";
    case builtin_type::U8:
        return "u8";
    case builtin_type::U16:
        return "u16";
    case builtin_type::U32:
        return "u32";
    case builtin_type::U64:
        return "u64";
    case builtin_type::USIZE:
        return "usize";
    case builtin_type::S8:
        return "s8";
    case builtin_type::S16:
        return "s16";
    case builtin_type::S32:
        return "s32";
    case builtin_type::S64:
        return "s64";
    case builtin_type::SSIZE:
        return "ssize";
    case builtin_type::F32:
        return "f32";
    case builtin_type::F64:
        return "f64";
    default:
        return "unknown";
    }
}

// Get builtin type from token type
inline constexpr builtin_type token_to_builtin_type(token_type token) {
    switch (token) {
    case token_type::BOOL:
        return builtin_type::BOOL;
    case token_type::CHAR:
        return builtin_type::CHAR;
    case token_type::STR:
        return builtin_type::STR;
    case token_type::U8:
        return builtin_type::U8;
    case token_type::U16:
        return builtin_type::U16;
    case token_type::U32:
        return builtin_type::U32;
    case token_type::U64:
        return builtin_type::U64;
    case token_type::USIZE:
        return builtin_type::USIZE;
    case token_type::S8:
        return builtin_type::S8;
    case token_type::S16:
        return builtin_type::S16;
    case token_type::S32:
        return builtin_type::S32;
    case token_type::S64:
        return builtin_type::S64;
    case token_type::SSIZE:
        return builtin_type::SSIZE;
    // Add more mappings as needed
    default:
        return builtin_type::VOID;
    }
}
