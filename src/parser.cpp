#include "parser.hpp"
#include "ast.hpp"
#include "lexer.hpp"
#include "memory.hpp"
#include "token.hpp"
#include <cstring>

// =============================================================================
// ENTRY POINTS
// =============================================================================

static ast_node *parse_translation_unit(parser &p);

// =============================================================================
// NEW LANGUAGE DECLARATION PARSING
// =============================================================================

static ast_node *parse_declaration(parser &p);
static ast_node *parse_variable_declaration(parser &p, const char *name, u32 name_len, ast_node *type,
                                            bool has_explicit_type);
static ast_node *parse_procedure_declaration(parser &p, const char *name, u32 name_len);

// Type parsing for new language
static ast_node *parse_type(parser &p);
static ast_node *parse_type_specifier(parser &p);

// Procedure parameters for new syntax: (param: type, param2: type)
static bool parse_parameter_list(parser &p, ast_parameter **params, u32 *param_count);

// =============================================================================
// STATEMENT PARSING
// =============================================================================

static ast_node *parse_statement(parser &p);
static ast_node *parse_compound_statement(parser &p);
static ast_node *parse_expression_statement(parser &p);
static ast_node *parse_if_statement(parser &p);
static ast_node *parse_while_statement(parser &p);
static ast_node *parse_return_statement(parser &p);

// =============================================================================
// EXPRESSION PARSING - FIXED TO HANDLE PROPER PRECEDENCE
// =============================================================================

static ast_node *parse_expression(parser &p);
static ast_node *parse_assignment(parser &p);
static ast_node *parse_logical_or(parser &p);
static ast_node *parse_logical_and(parser &p);
static ast_node *parse_equality(parser &p);
static ast_node *parse_relational(parser &p);
static ast_node *parse_additive(parser &p);
static ast_node *parse_multiplicative(parser &p);
static ast_node *parse_unary(parser &p);
static ast_node *parse_postfix(parser &p);
static ast_node *parse_primary(parser &p);
static ast_node *parse_module_declaration(parser &p);
static const char **parse_dotted_path(parser &p, u32 *path_len);
static bool parse_group_items(parser &p, ast_decl_use::use_item::group_item **items, u32 *item_count);
static bool parse_use_item(parser &p, ast_decl_use::use_item *item);
static ast_node *parse_use_declaration(parser &p);
static ast_node *parse_member_access(parser &p, ast_node *left);

bool parser_init(parser &p, struct lexer &lexer, arena &memory) {
    p.lexer = &lexer;
    p.memory = &memory;
    p.has_error = false;
    p.error = {.message = nullptr, .row = 0, .column = 0};

    return true;
}

void parser_deinit(parser &p) {
    p = {};
}

void parser_reset(parser &p) {
    arena_reset(*p.memory);
    p.has_error = false;
    p.error = {.message = nullptr, .row = 0, .column = 0};
}

ast_node *parse_program(parser &p) {
    // Get first token
    p.current_token = lexer_next_token(*p.lexer);
    return parse_translation_unit(p);
}

void parser_advance(parser &p) {
    if (p.current_token.type != token_type::END_OF_FILE) {
        p.current_token = lexer_next_token(*p.lexer);
    }
}

bool parser_match(parser &p, token_type type) {
    if (parser_check(p, type)) {
        parser_advance(p);
        return true;
    }
    return false;
}

bool parser_check(parser &p, token_type type) {
    return p.current_token.type == type;
}

token parser_consume(parser &p, token_type type, const char *error_msg) {
    if (parser_check(p, type)) {
        token prev = p.current_token;
        parser_advance(p);
        return prev;
    }

    parser_error(p, error_msg);
    return p.current_token;
}

void parser_error(parser &p, const char *message) {
    p.has_error = true;
    p.error = {.message = message, .row = p.current_token.row, .column = p.current_token.column};
}

void parser_synchronize(parser &p) {
    parser_advance(p);

    while (!parser_check(p, token_type::END_OF_FILE)) {
        // Look for statement boundaries or new declarations
        if (parser_check(p, token_type::NEWLINE)) {
            parser_advance(p);
            return;
        }

        // Look for identifiers that might start new declarations
        if (parser_check(p, token_type::IDENTIFIER)) {
            return;
        }

        // Control flow keywords
        switch (p.current_token.type) {
        case token_type::IF:
        case token_type::WHILE:
        case token_type::FOR:
        case token_type::RETURN:
            return;
        default:
            break;
        }

        parser_advance(p);
    }
}

static ast_node *parse_translation_unit(parser &p) {
    ast_node **declarations = nullptr;
    u32 declaration_count = 0;
    u32 declaration_capacity = 16;

    declarations = (ast_node **)arena_alloc_array(*p.memory, sizeof(ast_node *), declaration_capacity);
    if (!declarations) {
        parser_error(p, "Failed to allocate memory for declarations");
        return nullptr;
    }

    while (!parser_check(p, token_type::END_OF_FILE)) {
        // Skip newlines at top level
        if (parser_match(p, token_type::NEWLINE)) {
            continue;
        }

        ast_node *decl = parse_declaration(p);

        if (decl) {
            if (declaration_count >= declaration_capacity) {
                // Reallocate if needed
                declaration_capacity *= 2;
                ast_node **new_declarations =
                    (ast_node **)arena_alloc_array(*p.memory, sizeof(ast_node *), declaration_capacity);
                if (!new_declarations) {
                    parser_error(p, "Failed to reallocate memory for declarations");
                    break;
                }
                for (u32 i = 0; i < declaration_count; i++) {
                    new_declarations[i] = declarations[i];
                }
                declarations = new_declarations;
            }
            declarations[declaration_count++] = decl;
        }

        if (p.has_error) {
            parser_synchronize(p);
            p.has_error = false;
        }
    }

    ast_translation_unit *unit = (ast_translation_unit *)arena_alloc(*p.memory, sizeof(ast_translation_unit));
    if (!unit) {
        parser_error(p, "Failed to allocate memory for translation unit");
        return nullptr;
    }

    unit->root = {.type = ast_node_type::TRANSLATION_UNIT, .row = 1, .column = 1};
    unit->declarations = declarations;
    unit->declaration_count = declaration_count;

    return (ast_node *)unit;
}

// NEW: Parse declarations in the new syntax
static ast_node *parse_declaration(parser &p) {
    // Skip leading newlines
    while (parser_match(p, token_type::NEWLINE)) {
        // Skip
    }

    if (parser_check(p, token_type::END_OF_FILE)) {
        return nullptr;
    }

    // Expect identifier first: name :: ...
    token name_token = parser_consume(p, token_type::IDENTIFIER, "Expected identifier");
    if (p.has_error) return nullptr;

    // Must be followed by :: or :
    if (parser_match(p, token_type::COLON_COLON)) {
        // name :: value  or  name :: () -> type { }

        // Check if it's a procedure: name :: ()
        if (parser_check(p, token_type::LEFT_PAREN)) {
            return parse_procedure_declaration(p, name_token.value, name_token.stride);
        } else {
            // Variable with type inference: name :: value
            return parse_variable_declaration(p, name_token.value, name_token.stride, nullptr, false);
        }
    } else if (parser_match(p, token_type::COLON)) {
        // name : type : value
        ast_node *type = parse_type(p);
        if (!type) return nullptr;

        parser_consume(p, token_type::COLON, "Expected ':' after type in variable declaration");
        if (p.has_error) return nullptr;

        return parse_variable_declaration(p, name_token.value, name_token.stride, type, true);
    } else {
        parser_error(p, "Expected '::' or ':' after identifier");
        return nullptr;
    }
}

static ast_node *parse_variable_declaration(parser &p, const char *name, u32 name_len, ast_node *type,
                                            bool has_explicit_type) {
    // Parse the initializer value
    ast_node *initializer = parse_expression(p);
    if (!initializer) return nullptr;

    // Semicolons are optional in the new language
    parser_match(p, token_type::SEMICOLON);

    // Create variable declaration
    ast_decl_variable *var_decl = (ast_decl_variable *)arena_alloc(*p.memory, sizeof(ast_decl_variable));
    if (!var_decl) return nullptr;

    var_decl->root.type = ast_node_type::DECL_VARIABLE;
    var_decl->root.row = p.current_token.row;
    var_decl->root.column = p.current_token.column;
    var_decl->var_type = type; // Can be nullptr for type inference
    var_decl->name = name;
    var_decl->name_len = name_len;
    var_decl->initializer = initializer;
    var_decl->is_public = false; // For now, assume not public
    var_decl->has_explicit_type = has_explicit_type;

    return (ast_node *)var_decl;
}

static ast_node *parse_procedure_declaration(parser &p, const char *name, u32 name_len) {
    // name :: (params) -> return_type { body }

    parser_consume(p, token_type::LEFT_PAREN, "Expected '('");
    if (p.has_error) return nullptr;

    // Parse parameters
    ast_parameter *parameters = nullptr;
    u32 param_count = 0;

    if (!parser_check(p, token_type::RIGHT_PAREN)) {
        if (!parse_parameter_list(p, &parameters, &param_count)) {
            return nullptr;
        }
    }

    parser_consume(p, token_type::RIGHT_PAREN, "Expected ')'");
    if (p.has_error) return nullptr;

    // Parse return type: -> type
    ast_node *return_type = nullptr;
    bool has_explicit_return_type = false;

    if (parser_match(p, token_type::ARROW)) {
        return_type = parse_type(p);
        if (!return_type) return nullptr;
        has_explicit_return_type = true;
    } else {
        // Default to void if no return type specified
        ast_type_builtin *void_type = (ast_type_builtin *)arena_alloc(*p.memory, sizeof(ast_type_builtin));
        if (!void_type) return nullptr;
        void_type->root = {
            .type = ast_node_type::TYPE_BUILTIN, .row = p.current_token.row, .column = p.current_token.column};
        void_type->type = builtin_type::VOID;
        return_type = (ast_node *)void_type;
    }

    // Parse procedure body
    ast_node *body = nullptr;
    if (parser_check(p, token_type::LEFT_BRACE)) {
        body = parse_compound_statement(p);
        if (!body) return nullptr;
    }

    // Create procedure declaration
    ast_decl_procedure *proc_decl = (ast_decl_procedure *)arena_alloc(*p.memory, sizeof(ast_decl_procedure));
    if (!proc_decl) return nullptr;

    proc_decl->root = {
        .type = ast_node_type::DECL_PROCEDURE, .row = p.current_token.row, .column = p.current_token.column};
    proc_decl->return_type = return_type;
    proc_decl->name = name;
    proc_decl->name_len = name_len;
    proc_decl->parameters = parameters;
    proc_decl->parameter_count = param_count;
    proc_decl->body = body;
    proc_decl->is_public = false; // For now, assume not public
    proc_decl->has_explicit_return_type = has_explicit_return_type;

    return (ast_node *)proc_decl;
}

// =============================================================================
// TYPE PARSING FOR NEW LANGUAGE
// =============================================================================

static ast_node *parse_type(parser &p) {
    return parse_type_specifier(p);
}

static ast_node *parse_type_specifier(parser &p) {
    builtin_type type_kind;

    switch (p.current_token.type) {
    // New language types
    case token_type::BOOL:
        type_kind = builtin_type::BOOL;
        break;
    case token_type::CHAR:
        type_kind = builtin_type::CHAR;
        break;
    case token_type::STR:
        type_kind = builtin_type::STR;
        break;
    case token_type::U32:
        type_kind = builtin_type::U32;
        break;
    case token_type::F32: // You need to add these to token_type enum
        type_kind = builtin_type::F32;
        break;
    case token_type::S32:
        type_kind = builtin_type::S32;
        break;
    case token_type::U64:
        type_kind = builtin_type::U64;
        break;
    case token_type::S64:
        type_kind = builtin_type::S64;
        break;
    default:
        parser_error(p, "Expected type specifier");
        return nullptr;
    }

    u32 line = p.current_token.row;
    u32 col = p.current_token.column;
    parser_advance(p);

    // Create builtin type node
    ast_type_builtin *builtin = (ast_type_builtin *)arena_alloc(*p.memory, sizeof(ast_type_builtin));
    if (!builtin) return nullptr;

    builtin->root = {.type = ast_node_type::TYPE_BUILTIN, .row = line, .column = col};
    builtin->type = type_kind;

    return (ast_node *)builtin;
}

static bool parse_parameter_list(parser &p, ast_parameter **params, u32 *param_count) {
    // New syntax: (name: type, name2: type)
    u32 capacity = 8;
    ast_parameter *parameters = (ast_parameter *)arena_alloc_array(*p.memory, sizeof(ast_parameter), capacity);
    if (!parameters) return false;

    u32 count = 0;

    do {
        // Parse parameter name
        token name_tok = parser_consume(p, token_type::IDENTIFIER, "Expected parameter name");
        if (p.has_error) return false;

        parser_consume(p, token_type::COLON, "Expected ':' after parameter name");
        if (p.has_error) return false;

        // Parse parameter type
        ast_node *param_type = parse_type(p);
        if (!param_type) return false;

        if (count >= capacity) {
            parser_error(p, "Too many parameters");
            return false;
        }

        parameters[count].param_type = param_type;
        parameters[count].name = name_tok.value;
        parameters[count].name_len = name_tok.stride;
        parameters[count].has_default_value = false;
        parameters[count].default_value = nullptr;
        count++;

        // Check for comma
        if (parser_match(p, token_type::COMMA)) {
            continue;
        } else {
            break;
        }
    } while (!parser_check(p, token_type::RIGHT_PAREN) && !parser_check(p, token_type::END_OF_FILE));

    *params = parameters;
    *param_count = count;
    return true;
}

// =============================================================================
// STATEMENT PARSING (Updated for new syntax)
// =============================================================================

static ast_node *parse_statement(parser &p) {
    switch (p.current_token.type) {
    case token_type::LEFT_BRACE:
        return parse_compound_statement(p);
    case token_type::IF:
        return parse_if_statement(p);
    case token_type::WHILE:
        return parse_while_statement(p);
    case token_type::RETURN:
        return parse_return_statement(p);
    case token_type::IDENTIFIER: {
        // Check if this might be a variable declaration (IDENTIFIER :: or IDENTIFIER :)
        // We need to peek ahead without consuming tokens
        u32 saved_pos = p.lexer->current;
        u32 saved_row = p.lexer->row;
        u32 saved_col = p.lexer->column;
        token saved_token = p.current_token; // Save the current token

        lexer_advance(*p.lexer); // Skip the identifier
        token next = lexer_next_token(*p.lexer);

        // Restore lexer state completely
        p.lexer->current = saved_pos;
        p.lexer->row = saved_row;
        p.lexer->column = saved_col;
        p.current_token = saved_token; // Restore the saved token

        if (next.type == token_type::COLON_COLON || next.type == token_type::COLON) {
            // This is a variable declaration
            return parse_declaration(p);
        }
        // Otherwise fall through to expression statement
        return parse_expression_statement(p);
    }
    default:
        return parse_expression_statement(p);
    }
}

static ast_node *parse_compound_statement(parser &p) {
    u32 line = p.current_token.row;
    u32 col = p.current_token.column;

    parser_consume(p, token_type::LEFT_BRACE, "Expected '{'");
    if (p.has_error) return nullptr;

    ast_node **statements = nullptr;
    u32 statement_count = 0;
    u32 statement_capacity = 32;

    statements = (ast_node **)arena_alloc_array(*p.memory, sizeof(ast_node *), statement_capacity);
    if (!statements) {
        parser_error(p, "Failed to allocate memory for statements");
        return nullptr;
    }

    while (!parser_check(p, token_type::RIGHT_BRACE) && !parser_check(p, token_type::END_OF_FILE)) {
        // Skip newlines inside compound statements
        if (parser_match(p, token_type::NEWLINE)) {
            continue;
        }

        ast_node *stmt = parse_statement(p);

        if (stmt) {
            if (statement_count >= statement_capacity) {
                parser_error(p, "Too many statements in compound block");
                break;
            }
            statements[statement_count++] = stmt;
        }

        if (p.has_error) {
            parser_synchronize(p);
            p.has_error = false;
        }
    }

    parser_consume(p, token_type::RIGHT_BRACE, "Expected '}'");
    if (p.has_error) return nullptr;

    ast_stmt_compound *compound = (ast_stmt_compound *)arena_alloc(*p.memory, sizeof(ast_stmt_compound));
    if (!compound) return nullptr;

    compound->root = {.type = ast_node_type::STMT_COMPOUND, .row = line, .column = col};
    compound->statements = statements;
    compound->statement_count = statement_count;

    return (ast_node *)compound;
}

static ast_node *parse_if_statement(parser &p) {
    u32 line = p.current_token.row;
    u32 col = p.current_token.column;

    parser_consume(p, token_type::IF, "Expected 'if'");
    if (p.has_error) return nullptr;

    // New syntax: if condition { } else { } (no parentheses)
    ast_node *condition = parse_expression(p);
    if (!condition) return nullptr;

    ast_node *then_stmt = parse_statement(p);
    if (!then_stmt) return nullptr;

    ast_node *else_stmt = nullptr;
    if (parser_match(p, token_type::ELSE)) {
        else_stmt = parse_statement(p);
        if (!else_stmt) return nullptr;
    }

    ast_stmt_if *if_stmt = (ast_stmt_if *)arena_alloc(*p.memory, sizeof(ast_stmt_if));
    if (!if_stmt) return nullptr;

    if_stmt->root = {.type = ast_node_type::STMT_IF, .row = line, .column = col};
    if_stmt->condition = condition;
    if_stmt->then_stmt = then_stmt;
    if_stmt->else_stmt = else_stmt;

    return (ast_node *)if_stmt;
}

static ast_node *parse_return_statement(parser &p) {
    u32 line = p.current_token.row;
    u32 col = p.current_token.column;

    parser_consume(p, token_type::RETURN, "Expected 'return'");
    if (p.has_error) return nullptr;

    ast_node *value = nullptr;

    // Check if there's a return value (not followed by newline or brace)
    if (!parser_check(p, token_type::NEWLINE) && !parser_check(p, token_type::RIGHT_BRACE) &&
        !parser_check(p, token_type::SEMICOLON)) {
        value = parse_expression(p);
        if (!value) return nullptr;
    }

    // Semicolons are optional
    parser_match(p, token_type::SEMICOLON);

    ast_stmt_return *ret = (ast_stmt_return *)arena_alloc(*p.memory, sizeof(ast_stmt_return));
    if (!ret) return nullptr;

    ret->root = {.type = ast_node_type::STMT_RETURN, .row = line, .column = col};
    ret->value = value;

    return (ast_node *)ret;
}

static ast_node *parse_expression_statement(parser &p) {
    ast_node *expr = parse_expression(p);
    if (!expr) return nullptr;

    // Semicolons are optional in new language
    parser_match(p, token_type::SEMICOLON);

    ast_stmt_expression *expr_stmt = (ast_stmt_expression *)arena_alloc(*p.memory, sizeof(ast_stmt_expression));
    if (!expr_stmt) return nullptr;

    expr_stmt->root = {.type = ast_node_type::STMT_EXPRESSION, .row = expr->row, .column = expr->column};
    expr_stmt->expression = expr;

    return (ast_node *)expr_stmt;
}

static ast_node *parse_while_statement(parser &p) {
    u32 line = p.current_token.row;
    u32 col = p.current_token.column;

    parser_consume(p, token_type::WHILE, "Expected 'while'");
    if (p.has_error) return nullptr;

    // New syntax: while condition { } (no parentheses)
    ast_node *condition = parse_expression(p);
    if (!condition) return nullptr;

    ast_node *body = parse_statement(p);
    if (!body) return nullptr;

    ast_stmt_while *while_stmt = (ast_stmt_while *)arena_alloc(*p.memory, sizeof(ast_stmt_while));
    if (!while_stmt) return nullptr;

    while_stmt->root = {.type = ast_node_type::STMT_WHILE, .row = line, .column = col};
    while_stmt->condition = condition;
    while_stmt->body = body;

    return (ast_node *)while_stmt;
}

// =============================================================================
// EXPRESSION PARSING - PROPER PRECEDENCE HANDLING
// =============================================================================

static ast_node *parse_expression(parser &p) {
    return parse_assignment(p);
}

static ast_node *parse_assignment(parser &p) {
    ast_node *expr = parse_logical_or(p);

    // TODO: Handle assignment operators
    return expr;
}

static ast_node *parse_logical_or(parser &p) {
    ast_node *expr = parse_logical_and(p);

    while (parser_match(p, token_type::LOGICAL_OR)) {
        binary_op op = binary_op::LOGICAL_OR;
        ast_node *right = parse_logical_and(p);

        ast_expr_binary *binary = (ast_expr_binary *)arena_alloc(*p.memory, sizeof(ast_expr_binary));
        if (!binary) return nullptr;

        binary->root = {.type = ast_node_type::EXPR_BINARY, .row = expr->row, .column = expr->column};
        binary->op = op;
        binary->left = expr;
        binary->right = right;

        expr = (ast_node *)binary;
    }

    return expr;
}

static ast_node *parse_logical_and(parser &p) {
    ast_node *expr = parse_equality(p);

    while (parser_match(p, token_type::LOGICAL_AND)) {
        binary_op op = binary_op::LOGICAL_AND;
        ast_node *right = parse_equality(p);

        ast_expr_binary *binary = (ast_expr_binary *)arena_alloc(*p.memory, sizeof(ast_expr_binary));
        if (!binary) return nullptr;

        binary->root = {.type = ast_node_type::EXPR_BINARY, .row = expr->row, .column = expr->column};
        binary->op = op;
        binary->left = expr;
        binary->right = right;

        expr = (ast_node *)binary;
    }

    return expr;
}

static ast_node *parse_equality(parser &p) {
    ast_node *expr = parse_relational(p);

    while (parser_check(p, token_type::EQUAL_EQUAL) || parser_check(p, token_type::NOT_EQUAL)) {
        binary_op op = parser_match(p, token_type::EQUAL_EQUAL) ? binary_op::EQ : binary_op::NE;
        if (op == binary_op::NE) parser_advance(p); // consume NOT_EQUAL

        ast_node *right = parse_relational(p);

        ast_expr_binary *binary = (ast_expr_binary *)arena_alloc(*p.memory, sizeof(ast_expr_binary));
        if (!binary) return nullptr;

        binary->root = {.type = ast_node_type::EXPR_BINARY, .row = expr->row, .column = expr->column};
        binary->op = op;
        binary->left = expr;
        binary->right = right;

        expr = (ast_node *)binary;
    }

    return expr;
}

static ast_node *parse_relational(parser &p) {
    ast_node *expr = parse_additive(p);

    while (parser_check(p, token_type::LESS) || parser_check(p, token_type::LESS_EQUAL) ||
           parser_check(p, token_type::GREATER) || parser_check(p, token_type::GREATER_EQUAL)) {

        binary_op op;
        if (parser_match(p, token_type::LESS))
            op = binary_op::LT;
        else if (parser_match(p, token_type::LESS_EQUAL))
            op = binary_op::LE;
        else if (parser_match(p, token_type::GREATER))
            op = binary_op::GT;
        else if (parser_match(p, token_type::GREATER_EQUAL))
            op = binary_op::GE;

        ast_node *right = parse_additive(p);

        ast_expr_binary *binary = (ast_expr_binary *)arena_alloc(*p.memory, sizeof(ast_expr_binary));
        if (!binary) return nullptr;

        binary->root = {.type = ast_node_type::EXPR_BINARY, .row = expr->row, .column = expr->column};
        binary->op = op;
        binary->left = expr;
        binary->right = right;

        expr = (ast_node *)binary;
    }

    return expr;
}

static ast_node *parse_additive(parser &p) {
    ast_node *expr = parse_multiplicative(p);

    while (parser_check(p, token_type::PLUS) || parser_check(p, token_type::MINUS)) {
        binary_op op = parser_match(p, token_type::PLUS) ? binary_op::ADD : binary_op::SUB;
        if (op == binary_op::SUB) parser_advance(p); // consume MINUS

        ast_node *right = parse_multiplicative(p);

        ast_expr_binary *binary = (ast_expr_binary *)arena_alloc(*p.memory, sizeof(ast_expr_binary));
        if (!binary) return nullptr;

        binary->root = {.type = ast_node_type::EXPR_BINARY, .row = expr->row, .column = expr->column};
        binary->op = op;
        binary->left = expr;
        binary->right = right;

        expr = (ast_node *)binary;
    }

    return expr;
}

static ast_node *parse_multiplicative(parser &p) {
    ast_node *expr = parse_unary(p);

    while (parser_check(p, token_type::STAR) || parser_check(p, token_type::SLASH) ||
           parser_check(p, token_type::PERCENT)) {
        binary_op op;
        if (parser_match(p, token_type::STAR))
            op = binary_op::MUL;
        else if (parser_match(p, token_type::SLASH))
            op = binary_op::DIV;
        else if (parser_match(p, token_type::PERCENT))
            op = binary_op::MOD;

        ast_node *right = parse_unary(p);

        ast_expr_binary *binary = (ast_expr_binary *)arena_alloc(*p.memory, sizeof(ast_expr_binary));
        if (!binary) return nullptr;

        binary->root = {.type = ast_node_type::EXPR_BINARY, .row = expr->row, .column = expr->column};
        binary->op = op;
        binary->left = expr;
        binary->right = right;

        expr = (ast_node *)binary;
    }

    return expr;
}

static ast_node *parse_unary(parser &p) {
    if (parser_check(p, token_type::MINUS) || parser_check(p, token_type::PLUS) ||
        parser_check(p, token_type::EXCLAMATION)) {
        unary_op op;
        if (parser_match(p, token_type::MINUS))
            op = unary_op::MINUS;
        else if (parser_match(p, token_type::PLUS))
            op = unary_op::PLUS;
        else if (parser_match(p, token_type::EXCLAMATION))
            op = unary_op::NOT;

        u32 line = p.current_token.row;
        u32 col = p.current_token.column;

        ast_node *operand = parse_unary(p);

        ast_expr_unary *unary = (ast_expr_unary *)arena_alloc(*p.memory, sizeof(ast_expr_unary));
        if (!unary) return nullptr;

        unary->root = {.type = ast_node_type::EXPR_UNARY, .row = line, .column = col};
        unary->op = op;
        unary->operand = operand;

        return (ast_node *)unary;
    }

    return parse_postfix(p);
}

static ast_node *parse_postfix(parser &p) {
    ast_node *expr = parse_primary(p);

    // Handle procedure calls: identifier(args)
    while (parser_check(p, token_type::LEFT_PAREN)) {
        u32 line = p.current_token.row;
        u32 col = p.current_token.column;

        parser_advance(p); // consume '('

        // Parse arguments
        ast_node **arguments = nullptr;
        u32 argument_count = 0;
        u32 argument_capacity = 8;

        if (!parser_check(p, token_type::RIGHT_PAREN)) {
            arguments = (ast_node **)arena_alloc_array(*p.memory, sizeof(ast_node *), argument_capacity);
            if (!arguments) {
                parser_error(p, "Failed to allocate memory for arguments");
                return nullptr;
            }

            do {
                if (argument_count >= argument_capacity) {
                    parser_error(p, "Too many procedure arguments");
                    return nullptr;
                }

                ast_node *arg = parse_expression(p);
                if (!arg) return nullptr;

                arguments[argument_count++] = arg;

                if (parser_match(p, token_type::COMMA)) {
                    continue;
                } else {
                    break;
                }
            } while (!parser_check(p, token_type::RIGHT_PAREN) && !parser_check(p, token_type::END_OF_FILE));
        }

        parser_consume(p, token_type::RIGHT_PAREN, "Expected ')' after procedure arguments");
        if (p.has_error) return nullptr;

        // Create procedure call node
        ast_expr_call *call = (ast_expr_call *)arena_alloc(*p.memory, sizeof(ast_expr_call));
        if (!call) {
            parser_error(p, "Failed to allocate memory for procedure call");
            return nullptr;
        }

        call->root = {.type = ast_node_type::EXPR_CALL, .row = line, .column = col};
        call->procedure = expr;
        call->arguments = arguments;
        call->argument_count = argument_count;

        expr = (ast_node *)call;
    }

    return expr;
}

static ast_node *parse_primary(parser &p) {
    switch (p.current_token.type) {
    case token_type::NUMBER: {
        token tok = p.current_token;
        parser_advance(p);

        ast_expr_literal *lit = (ast_expr_literal *)arena_alloc(*p.memory, sizeof(ast_expr_literal));
        if (!lit) return nullptr;

        lit->root = {.type = ast_node_type::EXPR_LITERAL, .row = tok.row, .column = tok.column};
        lit->type = literal_type::INTEGER;
        lit->value = tok.value;
        lit->value_len = tok.stride;

        return (ast_node *)lit;
    }

    case token_type::STRING_LITERAL: {
        token tok = p.current_token;
        parser_advance(p);

        ast_expr_literal *lit = (ast_expr_literal *)arena_alloc(*p.memory, sizeof(ast_expr_literal));
        if (!lit) return nullptr;

        lit->root = {.type = ast_node_type::EXPR_LITERAL, .row = tok.row, .column = tok.column};
        lit->type = literal_type::STRING;
        lit->value = tok.value;
        lit->value_len = tok.stride;

        return (ast_node *)lit;
    }

    case token_type::TRUE:
    case token_type::FALSE: {
        token tok = p.current_token;
        parser_advance(p);

        ast_expr_literal *lit = (ast_expr_literal *)arena_alloc(*p.memory, sizeof(ast_expr_literal));
        if (!lit) return nullptr;

        lit->root = {.type = ast_node_type::EXPR_LITERAL, .row = tok.row, .column = tok.column};
        lit->type = literal_type::BOOLEAN;
        lit->value = tok.value;
        lit->value_len = tok.stride;

        return (ast_node *)lit;
    }

    case token_type::IDENTIFIER: {
        token tok = p.current_token;
        parser_advance(p);

        ast_expr_identifier *ident = (ast_expr_identifier *)arena_alloc(*p.memory, sizeof(ast_expr_identifier));
        if (!ident) return nullptr;

        ident->root = {.type = ast_node_type::EXPR_IDENTIFIER, .row = tok.row, .column = tok.column};
        ident->name = tok.value;
        ident->name_len = tok.stride;

        return (ast_node *)ident;
    }

    case token_type::LEFT_PAREN: {
        parser_advance(p);
        ast_node *expr = parse_expression(p);
        parser_consume(p, token_type::RIGHT_PAREN, "Expected ')' after expression");
        return expr;
    }

    default:
        parser_error(p, "Expected expression");
        return nullptr;
    }
}

static ast_node *parse_module_declaration(parser &p) {
    u32 line = p.current_token.row;
    u32 col = p.current_token.column;

    parser_consume(p, token_type::MOD, "Expected 'mod'");
    if (p.has_error) return nullptr;

    // Parse module name
    token name_token = parser_consume(p, token_type::IDENTIFIER, "Expected module name");
    if (p.has_error) return nullptr;

    // Parse module body
    parser_consume(p, token_type::LEFT_BRACE, "Expected '{' after module name");
    if (p.has_error) return nullptr;

    // Parse declarations inside the module
    ast_node **declarations = nullptr;
    u32 declaration_count = 0;
    u32 declaration_capacity = 16;

    declarations = (ast_node **)arena_alloc_array(*p.memory, sizeof(ast_node *), declaration_capacity);
    if (!declarations) {
        parser_error(p, "Failed to allocate memory for module declarations");
        return nullptr;
    }

    while (!parser_check(p, token_type::RIGHT_BRACE) && !parser_check(p, token_type::END_OF_FILE)) {
        // Skip newlines inside modules
        if (parser_match(p, token_type::NEWLINE)) {
            continue;
        }

        ast_node *decl = parse_declaration(p);
        if (decl) {
            if (declaration_count >= declaration_capacity) {
                // Reallocate if needed
                declaration_capacity *= 2;
                ast_node **new_declarations =
                    (ast_node **)arena_alloc_array(*p.memory, sizeof(ast_node *), declaration_capacity);
                if (!new_declarations) {
                    parser_error(p, "Failed to reallocate memory for module declarations");
                    break;
                }
                for (u32 i = 0; i < declaration_count; i++) {
                    new_declarations[i] = declarations[i];
                }
                declarations = new_declarations;
            }
            declarations[declaration_count++] = decl;
        }

        if (p.has_error) {
            parser_synchronize(p);
            p.has_error = false;
        }
    }

    parser_consume(p, token_type::RIGHT_BRACE, "Expected '}' after module body");
    if (p.has_error) return nullptr;

    // Create module declaration
    ast_decl_module *module_decl = (ast_decl_module *)arena_alloc(*p.memory, sizeof(ast_decl_module));
    if (!module_decl) return nullptr;

    // Copy module name
    char *name_copy = (char *)arena_alloc(*p.memory, name_token.stride + 1);
    if (!name_copy) return nullptr;
    strncpy(name_copy, name_token.value, name_token.stride);
    name_copy[name_token.stride] = '\0';

    module_decl->root = {.type = ast_node_type::DECL_MODULE, .row = line, .column = col};
    module_decl->name = name_copy;
    module_decl->name_len = name_token.stride;
    module_decl->declarations = declarations;
    module_decl->declaration_count = declaration_count;
    module_decl->parent_module = nullptr; // Filled during semantic analysis
    module_decl->full_path = nullptr;     // Filled during semantic analysis
    module_decl->path_depth = 0;          // Filled during semantic analysis

    return (ast_node *)module_decl;
}

// Parse dotted path: a.one.two
static const char **parse_dotted_path(parser &p, u32 *path_len) {
    const char **path = (const char **)arena_alloc_array(*p.memory, sizeof(const char *), 8);
    if (!path) return nullptr;

    u32 count = 0;
    u32 capacity = 8;

    do {
        token ident = parser_consume(p, token_type::IDENTIFIER, "Expected identifier in path");
        if (p.has_error) return nullptr;

        if (count >= capacity) {
            parser_error(p, "Path too long");
            return nullptr;
        }

        // Copy the identifier
        char *copied = (char *)arena_alloc(*p.memory, ident.stride + 1);
        if (!copied) return nullptr;
        strncpy(copied, ident.value, ident.stride);
        copied[ident.stride] = '\0';

        path[count++] = copied;

        if (parser_match(p, token_type::DOT)) {
            continue;
        } else {
            break;
        }
    } while (true);

    *path_len = count;
    return path;
}

// Parse group items: {one, two, three}
static bool parse_group_items(parser &p, ast_decl_use::use_item::group_item **items, u32 *item_count) {
    const u32 capacity = 16;
    ast_decl_use::use_item::group_item *group_items = (ast_decl_use::use_item::group_item *)arena_alloc_array(
        *p.memory, sizeof(ast_decl_use::use_item::group_item), capacity);
    if (!group_items) return false;

    u32 count = 0;

    do {
        if (count >= capacity) {
            parser_error(p, "Too many items in group import");
            return false;
        }

        token ident = parser_consume(p, token_type::IDENTIFIER, "Expected identifier in group");
        if (p.has_error) return false;

        // Copy identifier
        char *name_copy = (char *)arena_alloc(*p.memory, ident.stride + 1);
        if (!name_copy) return false;
        strncpy(name_copy, ident.value, ident.stride);
        name_copy[ident.stride] = '\0';

        group_items[count].name = name_copy;
        group_items[count].name_len = ident.stride;
        group_items[count].alias = nullptr; // TODO: Support aliases later
        group_items[count].alias_len = 0;
        count++;

        if (parser_match(p, token_type::COMMA)) {
            continue;
        } else {
            break;
        }
    } while (!parser_check(p, token_type::RIGHT_BRACE) && !parser_check(p, token_type::END_OF_FILE));

    *items = group_items;
    *item_count = count;
    return true;
}

// Parse single use item: a.one or a.{one, two}
static bool parse_use_item(parser &p, ast_decl_use::use_item *item) {
    // Parse the base path
    u32 path_len;
    const char **path = parse_dotted_path(p, &path_len);
    if (!path) return false;

    item->path = path;
    item->path_len = path_len;
    item->alias = nullptr;
    item->alias_len = 0;

    // Check for group import: a.{one, two}
    if (parser_match(p, token_type::LEFT_BRACE)) {
        item->is_group = true;

        if (!parse_group_items(p, &item->group_items, &item->group_count)) {
            return false;
        }

        parser_consume(p, token_type::RIGHT_BRACE, "Expected '}' after group items");
        return !p.has_error;
    } else {
        item->is_group = false;
        item->group_items = nullptr;
        item->group_count = 0;
        return true;
    }
}

// Parse use declaration: use a.one, b.{two, three}, c
static ast_node *parse_use_declaration(parser &p) {
    u32 line = p.current_token.row;
    u32 col = p.current_token.column;

    parser_consume(p, token_type::USE, "Expected 'use'");
    if (p.has_error) return nullptr;

    // Parse use items (comma-separated)
    const u32 capacity = 16;
    ast_decl_use::use_item *items =
        (ast_decl_use::use_item *)arena_alloc_array(*p.memory, sizeof(ast_decl_use::use_item), capacity);
    if (!items) {
        parser_error(p, "Failed to allocate memory for use items");
        return nullptr;
    }

    u32 count = 0;

    do {
        if (count >= capacity) {
            parser_error(p, "Too many use items");
            return nullptr;
        }

        if (!parse_use_item(p, &items[count])) {
            return nullptr;
        }
        count++;

        if (parser_match(p, token_type::COMMA)) {
            continue;
        } else {
            break;
        }
    } while (!parser_check(p, token_type::NEWLINE) && !parser_check(p, token_type::SEMICOLON) &&
             !parser_check(p, token_type::END_OF_FILE));

    // Semicolons are optional
    parser_match(p, token_type::SEMICOLON);

    // Create use declaration
    ast_decl_use *use_decl = (ast_decl_use *)arena_alloc(*p.memory, sizeof(ast_decl_use));
    if (!use_decl) return nullptr;

    use_decl->root = {.type = ast_node_type::DECL_USE, .row = line, .column = col};
    use_decl->items = items;
    use_decl->item_count = count;

    return (ast_node *)use_decl;
}

// Parse member access in expressions: a.one.function()
static ast_node *parse_member_access(parser &p, ast_node *left) {
    u32 line = p.current_token.row;
    u32 col = p.current_token.column;

    parser_consume(p, token_type::DOT, "Expected '.'");
    if (p.has_error) return nullptr;

    token member = parser_consume(p, token_type::IDENTIFIER, "Expected member name");
    if (p.has_error) return nullptr;

    // Copy member name
    char *member_copy = (char *)arena_alloc(*p.memory, member.stride + 1);
    if (!member_copy) return nullptr;
    strncpy(member_copy, member.value, member.stride);
    member_copy[member.stride] = '\0';

    // Create member access node
    ast_expr_member_access *access = (ast_expr_member_access *)arena_alloc(*p.memory, sizeof(ast_expr_member_access));
    if (!access) return nullptr;

    access->root = {.type = ast_node_type::EXPR_MEMBER_ACCESS, .row = line, .column = col};
    access->object = left;
    access->member = member_copy;
    access->member_len = member.stride;
    access->is_module_access = false;  // Filled during semantic analysis
    access->resolved_module = nullptr; // Filled during semantic analysis

    return (ast_node *)access;
}
