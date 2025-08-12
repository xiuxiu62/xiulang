#pragma once

#include "defines.hpp"
#include "token.hpp"

struct lexer {
    const char *source = nullptr;
    usize source_len = 0;
    usize current = 0;
    u32 row = 1, column = 1;
};

bool lexer_load(struct lexer &lexer, const char *source);

char lexer_peek(struct lexer const &lexer, u32 offset = 0);
char lexer_advance(struct lexer &lexer);
void lexer_skip_whitespace(struct lexer &lexer);
bool lexer_is_at_end(struct lexer &lexer);
// token lexer_scan_number(struct lexer &lexer);
// token lexer_scan_identifier(struct lexer &lexer);
// token lexer_scan_string(struct lexer &lexer);
// token lexer_scan_char(struct lexer &lexer);
token lexer_next_token(struct lexer &lexer);
