#include "lexer.hpp"
#include "token.hpp"
#include <cctype>
#include <cstring>
#include <string_view>
#include <unordered_map>

static token_type get_keyword_type(const char *word, u32 length);

bool lexer_load(struct lexer &l, const char *source) {
    if (!source) return false;
    l.source = source;
    l.source_len = strlen(source);
    l.current = 0;
    l.row = 1;
    l.column = 1;

    return true;
}

void lexer_reset(struct lexer &l) {
    l = {};
}

char lexer_peek(struct lexer const &l, u32 offset) {
    if (l.current + offset >= l.source_len) return '\0';
    return l.source[l.current + offset];
}

char lexer_advance(struct lexer &l) {
    if (lexer_is_at_end(l)) return '\0';

    char c = l.source[l.current++];
    if (c == '\n') {
        l.row++;
        l.column = 1;
    } else {
        l.column++;
    }

    return c;
}

void lexer_skip_whitespace(struct lexer &l) {
    while (!lexer_is_at_end(l)) {
        char c = lexer_peek(l);
        if (c == ' ' || c == '\t' || c == '\r') {
            lexer_advance(l);
        } else {
            break;
        }
    }
}

bool lexer_is_at_end(struct lexer &l) {
    return l.current >= l.source_len || l.source[l.current] == '\0';
}

token lexer_scan_number(struct lexer &l) {
    u32 start_row = l.row, start_col = l.column;
    const char *start = &l.source[l.current];

    // Scan integer part
    while (std::isdigit(lexer_peek(l))) {
        lexer_advance(l);
    }

    // Handle decimal numbers
    if (lexer_peek(l) == '.' && std::isdigit(lexer_peek(l, 1))) {
        lexer_advance(l); // consume '.'
        while (std::isdigit(lexer_peek(l))) {
            lexer_advance(l);
        }
    }

    u32 stride = &l.source[l.current] - start;

    return {.value = start, .type = token_type::NUMBER, .stride = stride, .row = start_row, .column = start_col};
}

token lexer_scan_identifier(struct lexer &l) {
    u32 start_row = l.row, start_col = l.column;
    const char *start = &l.source[l.current];

    while (std::isalnum(lexer_peek(l)) || lexer_peek(l) == '_') {
        lexer_advance(l);
    }

    u32 stride = &l.source[l.current] - start;
    token_type type = get_keyword_type(start, stride);

    return {.value = start, .type = type, .stride = stride, .row = start_row, .column = start_col};
}

// Enhanced string scanning with interpolation support
token lexer_scan_string(struct lexer &l) {
    u32 start_row = l.row, start_col = l.column;
    const char *start = &l.source[l.current];

    lexer_advance(l); // consume opening quote

    while (lexer_peek(l) != '"' && !lexer_is_at_end(l)) {
        if (lexer_peek(l) == '\\') {
            lexer_advance(l); // consume backslash
            lexer_advance(l); // consume escaped character
        } else if (lexer_peek(l) == '{') {
            // String interpolation detected - for now, just consume it
            // TODO: Handle proper string interpolation tokenization
            lexer_advance(l);
        } else {
            lexer_advance(l);
        }
    }

    if (lexer_peek(l) == '"') {
        lexer_advance(l); // consume closing quote
    }

    u32 stride = &l.source[l.current] - start;

    return {
        .value = start, .type = token_type::STRING_LITERAL, .stride = stride, .row = start_row, .column = start_col};
}

token lexer_scan_char(struct lexer &l) {
    u32 start_row = l.row, start_col = l.column;
    const char *start = &l.source[l.current];

    lexer_advance(l); // consume opening quote

    if (lexer_peek(l) == '\\') {
        lexer_advance(l); // consume backslash
        lexer_advance(l); // consume escaped character
    } else if (lexer_peek(l) != '\'' && !lexer_is_at_end(l)) {
        lexer_advance(l);
    }

    if (lexer_peek(l) == '\'') {
        lexer_advance(l); // consume closing quote
    }

    u32 stride = &l.source[l.current] - start;

    return {.value = start, .type = token_type::CHAR_LITERAL, .stride = stride, .row = start_row, .column = start_col};
}

token lexer_next_token(struct lexer &l) {
    lexer_skip_whitespace(l);

    if (lexer_is_at_end(l)) {
        return {.value = &l.source[l.current],
                .type = token_type::END_OF_FILE,
                .stride = 0,
                .row = l.row,
                .column = l.column};
    }

    char c = lexer_peek(l);
    u32 start_row = l.row, start_col = l.column;
    const char *start = &l.source[l.current];

    // Numbers
    if (std::isdigit(c)) {
        return lexer_scan_number(l);
    }

    // Identifiers and keywords
    if (std::isalpha(c) || c == '_') {
        return lexer_scan_identifier(l);
    }

    // String literals
    if (c == '"') {
        return lexer_scan_string(l);
    }

    // Character literals
    if (c == '\'') {
        return lexer_scan_char(l);
    }

    // Multi-character tokens first
    lexer_advance(l); // consume first character

    switch (c) {
    case ':':
        if (lexer_peek(l) == ':') {
            lexer_advance(l);
            return {start, token_type::COLON_COLON, 2, start_row, start_col};
        }
        return {start, token_type::COLON, 1, start_row, start_col};

    case '-':
        if (lexer_peek(l) == '>') {
            lexer_advance(l);
            return {start, token_type::ARROW, 2, start_row, start_col};
        }
        if (lexer_peek(l) == '=') {
            lexer_advance(l);
            return {start, token_type::SUB_ASSIGN, 2, start_row, start_col};
        }
        return {start, token_type::MINUS, 1, start_row, start_col};

    case '+':
        if (lexer_peek(l) == '=') {
            lexer_advance(l);
            return {start, token_type::ADD_ASSIGN, 2, start_row, start_col};
        }
        return {start, token_type::PLUS, 1, start_row, start_col};

    case '*':
        if (lexer_peek(l) == '=') {
            lexer_advance(l);
            return {start, token_type::MUL_ASSIGN, 2, start_row, start_col};
        }
        return {start, token_type::STAR, 1, start_row, start_col}; // FIXED: Use STAR instead of MULTIPLY

    case '/':
        if (lexer_peek(l) == '=') {
            lexer_advance(l);
            return {start, token_type::DIV_ASSIGN, 2, start_row, start_col};
        }
        return {start, token_type::SLASH, 1, start_row, start_col}; // FIXED: Use SLASH instead of DIVIDE

    case '%':
        if (lexer_peek(l) == '=') {
            lexer_advance(l);
            return {start, token_type::MOD_ASSIGN, 2, start_row, start_col};
        }
        return {start, token_type::PERCENT, 1, start_row, start_col}; // FIXED: Use PERCENT instead of MODULO

    case '=':
        if (lexer_peek(l) == '=') {
            lexer_advance(l);
            return {start, token_type::EQUAL_EQUAL, 2, start_row, start_col}; // FIXED: Use EQUAL_EQUAL, not EQUAL
        }
        return {start, token_type::EQUAL, 1, start_row, start_col}; // FIXED: Single = should be EQUAL (assignment)

    case '!':
        if (lexer_peek(l) == '=') {
            lexer_advance(l);
            return {start, token_type::NOT_EQUAL, 2, start_row, start_col};
        }
        return {start, token_type::EXCLAMATION, 1, start_row,
                start_col}; // FIXED: Use EXCLAMATION instead of LOGICAL_NOT

    case '<':
        if (lexer_peek(l) == '=') {
            lexer_advance(l);
            return {start, token_type::LESS_EQUAL, 2, start_row, start_col};
        }
        if (lexer_peek(l) == '<') {
            lexer_advance(l);
            return {start, token_type::LEFT_SHIFT, 2, start_row, start_col};
        }
        return {start, token_type::LESS, 1, start_row, start_col};

    case '>':
        if (lexer_peek(l) == '=') {
            lexer_advance(l);
            return {start, token_type::GREATER_EQUAL, 2, start_row, start_col};
        }
        if (lexer_peek(l) == '>') {
            lexer_advance(l);
            return {start, token_type::RIGHT_SHIFT, 2, start_row, start_col};
        }
        return {start, token_type::GREATER, 1, start_row, start_col};

    case '&':
        if (lexer_peek(l) == '&') {
            lexer_advance(l);
            return {start, token_type::LOGICAL_AND, 2, start_row, start_col};
        }
        return {start, token_type::BITWISE_AND, 1, start_row, start_col};

    case '|':
        if (lexer_peek(l) == '|') {
            lexer_advance(l);
            return {start, token_type::LOGICAL_OR, 2, start_row, start_col};
        }
        return {start, token_type::BITWISE_OR, 1, start_row, start_col};

    case '^':
        return {start, token_type::BITWISE_XOR, 1, start_row, start_col};

    // Single character tokens
    case '\n':
        return {start, token_type::NEWLINE, 1, start_row, start_col};
    case ';':
        return {start, token_type::SEMICOLON, 1, start_row, start_col};
    case ',':
        return {start, token_type::COMMA, 1, start_row, start_col};
    case '.':
        return {start, token_type::DOT, 1, start_row, start_col};
    case '(':
        return {start, token_type::LEFT_PAREN, 1, start_row, start_col};
    case ')':
        return {start, token_type::RIGHT_PAREN, 1, start_row, start_col};
    case '{':
        return {start, token_type::LEFT_BRACE, 1, start_row, start_col};
    case '}':
        return {start, token_type::RIGHT_BRACE, 1, start_row, start_col};
    case '[':
        return {start, token_type::LEFT_BRACKET, 1, start_row, start_col};
    case ']':
        return {start, token_type::RIGHT_BRACKET, 1, start_row, start_col};

    default:
        return {start, token_type::UNKNOWN, 1, start_row, start_col};
    }
}

static token_type get_keyword_type(const char *word, u32 length) {
    std::string_view word_view(word, length);

    static const std::unordered_map<std::string_view, token_type> keyword_to_token = {
        // Built-in types
        {"bool", token_type::BOOL},
        {"char", token_type::CHAR},
        {"str", token_type::STR},
        {"u8", token_type::U8},
        {"u16", token_type::U16},
        {"u32", token_type::U32},
        {"u64", token_type::U64},
        {"usize", token_type::USIZE},
        {"s8", token_type::S8},
        {"s16", token_type::S16},
        {"s32", token_type::S32},
        {"s64", token_type::S64},
        {"ssize", token_type::SSIZE},
        {"f32", token_type::F32},
        {"f64", token_type::F64},

        // Control flow
        {"if", token_type::IF},
        {"else", token_type::ELSE},
        {"while", token_type::WHILE},
        {"for", token_type::FOR},
        {"return", token_type::RETURN},
        {"break", token_type::BREAK},
        {"continue", token_type::CONTINUE},

        // Module system keywords
        {"mod", token_type::MOD},
        {"use", token_type::USE},

        // Other keywords
        {"pub", token_type::PUB},
        {"true", token_type::TRUE},
        {"false", token_type::FALSE},
    };

    auto it = keyword_to_token.find(word_view);
    return (it != keyword_to_token.end()) ? it->second : token_type::IDENTIFIER;
}
