#pragma once

#include "allocators.hpp"
#include "ast.hpp"
#include "token.hpp"

struct parser {
    struct lexer *lexer = nullptr;
    arena_allocator *memory = nullptr;
    token current_token = {};
    bool has_error = false;

    struct {
        const char *message = nullptr;
        u32 row = 0, column = 0;
    } error;
};

bool parser_init(struct parser &parser, struct lexer &lexer, arena_allocator &memory);
void parser_deinit(struct parser &parser);
void parser_reset(struct parser &parser);
ast_node *parse_program(parser &p);

void parser_advance(struct parser &parser);
bool parser_match(struct parser &parser, token_type type);
bool parser_check(struct parser &parser, token_type type);
token parser_consume(struct parser &parser, token_type type, const char *error_message);

void parser_error(struct parser &parser, const char *message);
void parser_synchronize(struct parser &parser);
