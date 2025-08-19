#include "token.hpp"
#include <string>

std::string token_type_to_string(token_type type) {
    switch (type) {
    // Literals
    case token_type::NUMBER:
        return "NUMBER";
    case token_type::STRING_LITERAL:
        return "STRING_LITERAL";
    case token_type::CHAR_LITERAL:
        return "CHAR_LITERAL";
    case token_type::IDENTIFIER:
        return "IDENTIFIER";

    // Built-in types
    case token_type::BOOL:
        return "BOOL";
    case token_type::CHAR:
        return "CHAR";
    case token_type::STR:
        return "STR";
    case token_type::U8:
        return "U8";
    case token_type::U16:
        return "U16";
    case token_type::U32:
        return "U32";
    case token_type::U64:
        return "U64";
    case token_type::USIZE:
        return "USIZE";
    case token_type::S8:
        return "S8";
    case token_type::S16:
        return "S16";
    case token_type::S32:
        return "S32";
    case token_type::S64:
        return "S64";
    case token_type::SSIZE:
        return "SSIZE";
    case token_type::F32:
        return "F32";
    case token_type::F64:
        return "F64";

    // Control flow keywords
    case token_type::IF:
        return "IF";
    case token_type::ELSE:
        return "ELSE";
    case token_type::WHILE:
        return "WHILE";
    case token_type::FOR:
        return "FOR";
    case token_type::RETURN:
        return "RETURN";
    case token_type::BREAK:
        return "BREAK";
    case token_type::CONTINUE:
        return "CONTINUE";
    case token_type::DEFER:
        return "DEFER";

    // Module system keywords
    case token_type::MOD:
        return "MOD";
    case token_type::USE:
        return "USE";

    // Other keywords
    case token_type::PUB:
        return "PUB";
    case token_type::TRUE:
        return "TRUE";
    case token_type::FALSE:
        return "FALSE";

    // Arithmetic operators
    case token_type::PLUS:
        return "PLUS";
    case token_type::MINUS:
        return "MINUS";
    case token_type::STAR:
        return "STAR";
    case token_type::SLASH:
        return "SLASH";
    case token_type::PERCENT:
        return "PERCENT";

    // Comparison operators
    case token_type::EQUAL:
        return "EQUAL";
    case token_type::EQUAL_EQUAL:
        return "EQUAL_EQUAL";
    case token_type::NOT_EQUAL:
        return "NOT_EQUAL";
    case token_type::LESS:
        return "LESS";
    case token_type::GREATER:
        return "GREATER";
    case token_type::LESS_EQUAL:
        return "LESS_EQUAL";
    case token_type::GREATER_EQUAL:
        return "GREATER_EQUAL";

    // Logical operators
    case token_type::LOGICAL_AND:
        return "LOGICAL_AND";
    case token_type::LOGICAL_OR:
        return "LOGICAL_OR";
    case token_type::EXCLAMATION:
        return "EXCLAMATION";

    // Bitwise operators
    case token_type::BITWISE_AND:
        return "BITWISE_AND";
    case token_type::BITWISE_OR:
        return "BITWISE_OR";
    case token_type::BITWISE_XOR:
        return "BITWISE_XOR";
    case token_type::BITWISE_NOT:
        return "BITWISE_NOT";
    case token_type::LEFT_SHIFT:
        return "LEFT_SHIFT";
    case token_type::RIGHT_SHIFT:
        return "RIGHT_SHIFT";

    // Assignment operators
    case token_type::ASSIGN:
        return "ASSIGN";
    case token_type::ADD_ASSIGN:
        return "ADD_ASSIGN";
    case token_type::SUB_ASSIGN:
        return "SUB_ASSIGN";
    case token_type::MUL_ASSIGN:
        return "MUL_ASSIGN";
    case token_type::DIV_ASSIGN:
        return "DIV_ASSIGN";
    case token_type::MOD_ASSIGN:
        return "MOD_ASSIGN";

    // Punctuation
    case token_type::SEMICOLON:
        return "SEMICOLON";
    case token_type::COMMA:
        return "COMMA";
    case token_type::DOT:
        return "DOT";
    case token_type::LEFT_PAREN:
        return "LEFT_PAREN";
    case token_type::RIGHT_PAREN:
        return "RIGHT_PAREN";
    case token_type::LEFT_BRACE:
        return "LEFT_BRACE";
    case token_type::RIGHT_BRACE:
        return "RIGHT_BRACE";
    case token_type::LEFT_BRACKET:
        return "LEFT_BRACKET";
    case token_type::RIGHT_BRACKET:
        return "RIGHT_BRACKET";

    // Language-specific tokens
    case token_type::COLON_COLON:
        return "COLON_COLON";
    case token_type::COLON:
        return "COLON";
    case token_type::COLON_EQUAL:
        return "COLON_EQUAL";
    case token_type::ARROW:
        return "ARROW";

    // String interpolation
    case token_type::INTERPOLATION_START:
        return "INTERPOLATION_START";
    case token_type::INTERPOLATION_END:
        return "INTERPOLATION_END";

    // Special tokens
    case token_type::NEWLINE:
        return "NEWLINE";
    case token_type::END_OF_FILE:
        return "END_OF_FILE";
    case token_type::UNKNOWN:
    default:
        return "UNKNOWN";
    }
}

std::string token_to_string(struct token const &t) {
    return token_type_to_string(t.type) + ": " + std::string(t.value, t.stride);
}
