#pragma once

#include "defines.hpp"
#include <string>

enum class token_type : u32 {
    // Literals
    NUMBER,
    STRING_LITERAL,
    CHAR_LITERAL,
    IDENTIFIER,

    // Keywords - Built-in types
    BOOL,
    CHAR,
    STR,
    U8,
    U16,
    U32,
    U64,
    USIZE,
    S8,
    S16,
    S32,
    S64,
    SSIZE,
    F32,
    F64,

    // Keywords - Control flow
    IF,
    ELSE,
    WHILE,
    FOR,
    RETURN,
    BREAK,
    CONTINUE,

    // Keywords - Module system
    MOD,
    USE,

    // Keywords - Other
    PUB,
    TRUE,
    FALSE,

    // Arithmetic operators
    PLUS,     // +
    MINUS,    // -
    MULTIPLY, // * (renamed from MULTIPLY)
    STAR,     // * (alias for compatibility)
    DIVIDE,   // /
    SLASH,    // / (alias for compatibility)
    MODULO,   // %
    PERCENT,  // % (alias for compatibility)

    // Comparison operators
    EQUAL,         // =
    EQUAL_EQUAL,   // == (added)
    NOT_EQUAL,     // !=
    LESS,          // <
    GREATER,       // >
    LESS_EQUAL,    // <=
    GREATER_EQUAL, // >=

    // Logical operators
    LOGICAL_AND, // &&
    LOGICAL_OR,  // ||
    LOGICAL_NOT, // ! (renamed)
    EXCLAMATION, // ! (alias for compatibility)

    // Bitwise operators
    BITWISE_AND, // &
    BITWISE_OR,  // |
    BITWISE_XOR, // ^
    LEFT_SHIFT,  // <<
    RIGHT_SHIFT, // >>

    // Assignment operators
    ASSIGN,     // =
    ADD_ASSIGN, // +=
    SUB_ASSIGN, // -=
    MUL_ASSIGN, // *=
    DIV_ASSIGN, // /=
    MOD_ASSIGN, // %=

    // Punctuation
    SEMICOLON,     // ;
    COMMA,         // ,
    DOT,           // .
    LEFT_PAREN,    // (
    RIGHT_PAREN,   // )
    LEFT_BRACE,    // {
    RIGHT_BRACE,   // }
    LEFT_BRACKET,  // [
    RIGHT_BRACKET, // ]

    // New language-specific tokens
    COLON_COLON, // ::
    COLON,       // :
    ARROW,       // ->

    // String interpolation
    INTERPOLATION_START, // "{
    INTERPOLATION_END,   // }"

    NEWLINE = u32_max - 2,
    END_OF_FILE = u32_max - 1,
    UNKNOWN = u32_max,
};

struct token {
    const char *value = nullptr;
    token_type type = token_type::UNKNOWN;
    u32 stride = 0;
    u32 row = 0, column = 0;
};

std::string token_type_to_string(token_type type);
std::string token_to_string(struct token const &token);
