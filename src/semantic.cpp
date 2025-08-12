#include "semantic.hpp"
#include "ast.hpp"
#include "logger.hpp"
#include "memory.hpp"
#include <cstring>

bool semantic_analyzer_init(semantic_analyzer &analyzer, arena &memory) {
    analyzer.memory = &memory;
    analyzer.current_scope = nullptr;
    analyzer.global_scope = nullptr;
    analyzer.has_error = false;
    analyzer.error_message = nullptr;
    analyzer.error_row = 0;
    analyzer.error_column = 0;
    analyzer.current_procedure = nullptr;
    analyzer.expected_return_type = nullptr;
    analyzer.current_stack_offset = 0;
    analyzer.scope_counter = 0;
    analyzer.builtin_procedures = nullptr;
    analyzer.builtin_procedure_count = 0;

    // Create global scope
    analyzer.global_scope = symbol_table_create(analyzer, nullptr);
    if (!analyzer.global_scope) {
        return false;
    }
    analyzer.current_scope = analyzer.global_scope;

    // Register builtin procedures
    register_builtin_procedures(analyzer);

    return true;
}

void semantic_analyzer_deinit(semantic_analyzer &analyzer) {
    if (analyzer.global_scope) {
        symbol_table_destroy(analyzer.global_scope);
    }
    analyzer = {};
}

bool analyze(semantic_analyzer &analyzer, ast_node *program) {
    if (!program || program->type != ast_node_type::TRANSLATION_UNIT) {
        semantic_error(analyzer, "Invalid program AST", 0, 0);
        return false;
    }

    return analyze_translation_unit(analyzer, (ast_translation_unit *)program);
}

// =============================================================================
// SYMBOL TABLE MANAGEMENT
// =============================================================================

symbol_table *symbol_table_create(semantic_analyzer &analyzer, symbol_table *parent) {
    symbol_table *table = (symbol_table *)arena_alloc(*analyzer.memory, sizeof(symbol_table));
    if (!table) return nullptr;

    table->symbols = nullptr;
    table->count = 0;
    table->capacity = 16;
    table->parent = parent;
    table->scope_depth = parent ? parent->scope_depth + 1 : 0;

    table->symbols = (symbol *)arena_alloc_array(*analyzer.memory, sizeof(symbol), table->capacity);
    if (!table->symbols) return nullptr;

    return table;
}

void symbol_table_destroy(symbol_table *table) {
    // Memory is managed by arena, so just clear the structure
    if (table) {
        table->symbols = nullptr;
        table->count = 0;
        table->capacity = 0;
    }
}

bool symbol_table_add(semantic_analyzer &analyzer, symbol_table *table, const symbol &sym) {
    if (!table) return false;

    // Check for duplicate in current scope
    symbol *existing = symbol_table_lookup_current_scope(table, sym.name, sym.name_len);
    if (existing) {
        semantic_error(analyzer, "Symbol already defined in current scope", sym.declaration->row,
                       sym.declaration->column);
        return false;
    }

    // Resize if needed
    if (table->count >= table->capacity) {
        u32 new_capacity = table->capacity * 2;
        symbol *new_symbols = (symbol *)arena_alloc_array(*analyzer.memory, sizeof(symbol), new_capacity);
        if (!new_symbols) return false;

        // Copy existing symbols
        for (u32 i = 0; i < table->count; i++) {
            new_symbols[i] = table->symbols[i];
        }

        table->symbols = new_symbols;
        table->capacity = new_capacity;
    }

    table->symbols[table->count] = sym;
    table->count++;
    return true;
}

symbol *symbol_table_lookup(symbol_table *table, const char *name, u32 name_len) {
    while (table) {
        for (u32 i = 0; i < table->count; i++) {
            if (symbol_names_equal(table->symbols[i].name, table->symbols[i].name_len, name, name_len)) {
                return &table->symbols[i];
            }
        }
        table = table->parent;
    }
    return nullptr;
}

symbol *symbol_table_lookup_current_scope(symbol_table *table, const char *name, u32 name_len) {
    if (!table) return nullptr;

    for (u32 i = 0; i < table->count; i++) {
        if (symbol_names_equal(table->symbols[i].name, table->symbols[i].name_len, name, name_len)) {
            return &table->symbols[i];
        }
    }
    return nullptr;
}

// =============================================================================
// SCOPE MANAGEMENT
// =============================================================================

void push_scope(semantic_analyzer &analyzer) {
    symbol_table *new_scope = symbol_table_create(analyzer, analyzer.current_scope);
    if (new_scope) {
        analyzer.current_scope = new_scope;
        analyzer.scope_counter++;
    }
}

void pop_scope(semantic_analyzer &analyzer) {
    if (analyzer.current_scope && analyzer.current_scope->parent) {
        analyzer.current_scope = analyzer.current_scope->parent;
    }
}

// =============================================================================
// ANALYSIS
// =============================================================================

bool analyze_translation_unit(semantic_analyzer &analyzer, ast_translation_unit *unit) {
    info("🔍 Analyzing translation unit with %u declarations", unit->declaration_count);

    // First pass: Collect all procedure and global variable declarations
    for (u32 i = 0; i < unit->declaration_count; i++) {
        if (!analyze_declaration(analyzer, unit->declarations[i])) {
            return false;
        }
    }

    // Second pass: Analyze procedure bodies
    for (u32 i = 0; i < unit->declaration_count; i++) {
        ast_node *decl = unit->declarations[i];
        if (decl->type == ast_node_type::DECL_PROCEDURE) {
            ast_decl_procedure *proc = (ast_decl_procedure *)decl;
            if (proc->body) {
                analyzer.current_procedure = proc;
                analyzer.expected_return_type = proc->return_type;

                push_scope(analyzer);

                // Add parameters to procedure scope
                for (u32 j = 0; j < proc->parameter_count; j++) {
                    symbol param_symbol = {};
                    param_symbol.name = proc->parameters[j].name;
                    param_symbol.name_len = proc->parameters[j].name_len;
                    param_symbol.type = symbol_type::PARAMETER;
                    param_symbol.type_node = proc->parameters[j].param_type;
                    param_symbol.declaration = (ast_node *)proc; // Point to procedure
                    param_symbol.info.variable.stack_offset = analyzer.current_stack_offset;
                    param_symbol.info.variable.is_initialized = true;
                    param_symbol.info.variable.is_global = false;

                    analyzer.current_stack_offset += 8; // Assume 8-byte parameters

                    if (!symbol_table_add(analyzer, analyzer.current_scope, param_symbol)) {
                        return false;
                    }
                }

                bool result = analyze_statement(analyzer, proc->body);
                pop_scope(analyzer);

                analyzer.current_procedure = nullptr;
                analyzer.expected_return_type = nullptr;
                analyzer.current_stack_offset = 0;

                if (!result) return false;
            }
        }
    }

    info("✅ Semantic analysis completed successfully");
    return true;
}

bool analyze_declaration(semantic_analyzer &analyzer, ast_node *decl) {
    switch (decl->type) {
    case ast_node_type::DECL_VARIABLE:
        return analyze_variable_declaration(analyzer, (ast_decl_variable *)decl);
    case ast_node_type::DECL_PROCEDURE:
        return analyze_procedure_declaration(analyzer, (ast_decl_procedure *)decl);
    default:
        semantic_error_at_node(analyzer, "Unknown declaration type", decl);
        return false;
    }
}

bool analyze_variable_declaration(semantic_analyzer &analyzer, ast_decl_variable *var_decl) {
    // Analyze initializer if present
    if (var_decl->initializer) {
        if (!analyze_expression(analyzer, var_decl->initializer)) {
            return false;
        }
    }

    // Infer type if not explicitly specified
    ast_node *var_type = var_decl->var_type;
    if (!var_type && var_decl->initializer) {
        var_type = infer_expression_type(analyzer, var_decl->initializer);
        if (!var_type) {
            semantic_error_at_node(analyzer, "Cannot infer type for variable", (ast_node *)var_decl);
            return false;
        }
    }

    // Create symbol
    symbol var_symbol = {};
    var_symbol.name = var_decl->name;
    var_symbol.name_len = var_decl->name_len;
    var_symbol.type = symbol_type::VARIABLE;
    var_symbol.type_node = var_type;
    var_symbol.declaration = (ast_node *)var_decl;
    var_symbol.info.variable.is_initialized = var_decl->initializer != nullptr;
    var_symbol.info.variable.is_global = analyzer.current_scope == analyzer.global_scope;

    if (var_symbol.info.variable.is_global) {
        var_symbol.info.variable.stack_offset = 0; // Global variables don't use stack offset
    } else {
        var_symbol.info.variable.stack_offset = analyzer.current_stack_offset;
        analyzer.current_stack_offset += 8; // Assume 8 bytes per variable
    }

    return symbol_table_add(analyzer, analyzer.current_scope, var_symbol);
}

bool analyze_procedure_declaration(semantic_analyzer &analyzer, ast_decl_procedure *proc_decl) {
    // Create procedure symbol
    symbol proc_symbol = {};
    proc_symbol.name = proc_decl->name;
    proc_symbol.name_len = proc_decl->name_len;
    proc_symbol.type = symbol_type::PROCEDURE;
    proc_symbol.type_node = proc_decl->return_type;
    proc_symbol.declaration = (ast_node *)proc_decl;
    proc_symbol.info.procedure.parameter_count = proc_decl->parameter_count;
    proc_symbol.info.procedure.parameters = proc_decl->parameters;
    proc_symbol.info.procedure.return_type = proc_decl->return_type;
    proc_symbol.info.procedure.has_body = proc_decl->body != nullptr;

    return symbol_table_add(analyzer, analyzer.global_scope, proc_symbol);
}

bool analyze_statement(semantic_analyzer &analyzer, ast_node *stmt) {
    switch (stmt->type) {
    case ast_node_type::STMT_COMPOUND:
        return analyze_compound_statement(analyzer, (ast_stmt_compound *)stmt);
    case ast_node_type::STMT_IF:
        return analyze_if_statement(analyzer, (ast_stmt_if *)stmt);
    case ast_node_type::STMT_RETURN:
        return analyze_return_statement(analyzer, (ast_stmt_return *)stmt);
    case ast_node_type::STMT_EXPRESSION:
        return analyze_expression_statement(analyzer, (ast_stmt_expression *)stmt);
    default:
        semantic_error_at_node(analyzer, "Unknown statement type", stmt);
        return false;
    }
}

bool analyze_compound_statement(semantic_analyzer &analyzer, ast_stmt_compound *compound) {
    push_scope(analyzer);

    bool result = true;
    for (u32 i = 0; i < compound->statement_count; i++) {
        ast_node *stmt = compound->statements[i];

        // Handle variable declarations that appear as statements
        if (stmt->type == ast_node_type::DECL_VARIABLE) {
            if (!analyze_variable_declaration(analyzer, (ast_decl_variable *)stmt)) {
                result = false;
                break;
            }
        } else {
            if (!analyze_statement(analyzer, stmt)) {
                result = false;
                break;
            }
        }
    }

    pop_scope(analyzer);
    return result;
}

bool analyze_if_statement(semantic_analyzer &analyzer, ast_stmt_if *if_stmt) {
    // Analyze condition
    if (!analyze_expression(analyzer, if_stmt->condition)) {
        return false;
    }

    // TODO: Check that condition is boolean

    // Analyze then statement
    if (!analyze_statement(analyzer, if_stmt->then_stmt)) {
        return false;
    }

    // Analyze else statement if present
    if (if_stmt->else_stmt) {
        return analyze_statement(analyzer, if_stmt->else_stmt);
    }

    return true;
}

bool analyze_return_statement(semantic_analyzer &analyzer, ast_stmt_return *return_stmt) {
    if (return_stmt->value) {
        if (!analyze_expression(analyzer, return_stmt->value)) {
            return false;
        }

        // TODO: Check return type compatibility
        if (analyzer.expected_return_type) {
            ast_node *return_type = infer_expression_type(analyzer, return_stmt->value);
            if (return_type && !types_compatible(analyzer.expected_return_type, return_type)) {
                semantic_error_at_node(analyzer, "Return type mismatch", (ast_node *)return_stmt);
                return false;
            }
        }
    }

    return true;
}

bool analyze_expression_statement(semantic_analyzer &analyzer, ast_stmt_expression *expr_stmt) {
    return analyze_expression(analyzer, expr_stmt->expression);
}

bool analyze_expression(semantic_analyzer &analyzer, ast_node *expr) {
    switch (expr->type) {
    case ast_node_type::EXPR_BINARY:
        return analyze_binary_expression(analyzer, (ast_expr_binary *)expr);
    case ast_node_type::EXPR_UNARY:
        return analyze_unary_expression(analyzer, (ast_expr_unary *)expr);
    case ast_node_type::EXPR_LITERAL:
        return analyze_literal_expression(analyzer, (ast_expr_literal *)expr);
    case ast_node_type::EXPR_IDENTIFIER:
        return analyze_identifier_expression(analyzer, (ast_expr_identifier *)expr);
    case ast_node_type::EXPR_CALL:
        return analyze_call_expression(analyzer, (ast_expr_call *)expr);
    default:
        semantic_error_at_node(analyzer, "Unknown expression type", expr);
        return false;
    }
}

bool analyze_binary_expression(semantic_analyzer &analyzer, ast_expr_binary *binary) {
    if (!analyze_expression(analyzer, binary->left)) {
        return false;
    }
    if (!analyze_expression(analyzer, binary->right)) {
        return false;
    }

    // TODO: Type checking for binary operations
    return true;
}

bool analyze_unary_expression(semantic_analyzer &analyzer, ast_expr_unary *unary) {
    return analyze_expression(analyzer, unary->operand);
}

bool analyze_literal_expression(semantic_analyzer &analyzer, ast_expr_literal *literal) {
    // Literals are always valid
    return true;
}

bool analyze_identifier_expression(semantic_analyzer &analyzer, ast_expr_identifier *identifier) {
    // First check the symbol table
    symbol *sym = symbol_table_lookup(analyzer.current_scope, identifier->name, identifier->name_len);
    if (sym) {
        // TODO: Check if variable is initialized before use
        return true;
    }

    // If not found in symbol table, check if it's a builtin procedure
    if (is_builtin_procedure_call(analyzer, identifier->name, identifier->name_len)) {
        return true;
    }

    semantic_error_at_node(analyzer, "Undefined identifier", (ast_node *)identifier);
    return false;
}

// bool analyze_identifier_expression(semantic_analyzer &analyzer, ast_expr_identifier *identifier) {
//     symbol *sym = symbol_table_lookup(analyzer.current_scope, identifier->name, identifier->name_len);
//     if (!sym) {
//         semantic_error_at_node(analyzer, "Undefined identifier", (ast_node *)identifier);
//         return false;
//     }

//     // TODO: Check if variable is initialized before use
//     return true;
// }

// =============================================================================
// TYPE CHECKING AND INFERENCE
// =============================================================================

ast_node *infer_expression_type(semantic_analyzer &analyzer, ast_node *expr) {
    switch (expr->type) {
    case ast_node_type::EXPR_LITERAL: {
        ast_expr_literal *lit = (ast_expr_literal *)expr;
        ast_type_builtin *builtin = (ast_type_builtin *)arena_alloc(*analyzer.memory, sizeof(ast_type_builtin));
        if (!builtin) return nullptr;

        builtin->root = {.type = ast_node_type::TYPE_BUILTIN, .row = expr->row, .column = expr->column};

        switch (lit->type) {
        case literal_type::INTEGER:
            builtin->type = builtin_type::S32;
            break;
        case literal_type::FLOAT:
            builtin->type = builtin_type::F32;
            break;
        case literal_type::STRING:
            builtin->type = builtin_type::STR;
            break;
        case literal_type::BOOLEAN:
            builtin->type = builtin_type::BOOL;
            break;
        case literal_type::CHARACTER:
            builtin->type = builtin_type::CHAR;
            break;
        default:
            return nullptr;
        }

        return (ast_node *)builtin;
    }
    case ast_node_type::EXPR_IDENTIFIER: {
        ast_expr_identifier *ident = (ast_expr_identifier *)expr;
        symbol *sym = symbol_table_lookup(analyzer.current_scope, ident->name, ident->name_len);
        return sym ? sym->type_node : nullptr;
    }
    default:
        // TODO: Implement for other expression types
        return nullptr;
    }
}

bool types_compatible(ast_node *type1, ast_node *type2) {
    if (!type1 || !type2) return false;

    if (type1->type != type2->type) return false;

    if (type1->type == ast_node_type::TYPE_BUILTIN) {
        ast_type_builtin *builtin1 = (ast_type_builtin *)type1;
        ast_type_builtin *builtin2 = (ast_type_builtin *)type2;
        return builtin1->type == builtin2->type;
    }

    // TODO: Handle other type comparisons
    return false;
}

// =============================================================================
// ERROR REPORTING
// =============================================================================

void semantic_error(semantic_analyzer &analyzer, const char *message, u32 row, u32 col) {
    analyzer.has_error = true;
    analyzer.error_message = message;
    analyzer.error_row = row;
    analyzer.error_column = col;
}

void semantic_error_at_node(semantic_analyzer &analyzer, const char *message, ast_node *node) {
    semantic_error(analyzer, message, node->row, node->column);
}

// =============================================================================
// UTILITY
// =============================================================================

bool symbol_names_equal(const char *name1, u32 len1, const char *name2, u32 len2) {
    return len1 == len2 && strncmp(name1, name2, len1) == 0;
}

const char *symbol_type_to_string(symbol_type type) {
    switch (type) {
    case symbol_type::VARIABLE:
        return "variable";
    case symbol_type::PROCEDURE:
        return "procedure";
    case symbol_type::PARAMETER:
        return "parameter";
    case symbol_type::STRUCT:
        return "struct";
    case symbol_type::ENUM:
        return "enum";
    case symbol_type::UNION:
        return "union";
    default:
        return "unknown";
    }
}

ast_node *create_builtin_type(semantic_analyzer &analyzer, builtin_type type) {
    ast_type_builtin *builtin = (ast_type_builtin *)arena_alloc(*analyzer.memory, sizeof(ast_type_builtin));
    if (!builtin) return nullptr;

    builtin->root = {.type = ast_node_type::TYPE_BUILTIN, .row = 0, .column = 0};
    builtin->type = type;

    return (ast_node *)builtin;
}

void register_builtin_procedures(semantic_analyzer &analyzer) {
    // Allocate space for builtin procedures
    analyzer.builtin_procedure_count = 2; // print and println
    analyzer.builtin_procedures = (builtin_procedure *)arena_alloc_array(*analyzer.memory, sizeof(builtin_procedure),
                                                                         analyzer.builtin_procedure_count);

    if (!analyzer.builtin_procedures) return;

    // Create common types
    ast_node *str_type = create_builtin_type(analyzer, builtin_type::STR);
    ast_node *void_type = create_builtin_type(analyzer, builtin_type::VOID);

    // Register print procedure: print(message: str) -> void
    ast_node **print_params = (ast_node **)arena_alloc_array(*analyzer.memory, sizeof(ast_node *), 1);
    if (print_params) {
        print_params[0] = str_type;
    }

    analyzer.builtin_procedures[0] = {.name = "print",
                                      .type = builtin_procedure_type::print,
                                      .param_count = 1,
                                      .param_types = print_params,
                                      .return_type = void_type};

    // Register println procedure: println(message: str) -> void
    ast_node **println_params = (ast_node **)arena_alloc_array(*analyzer.memory, sizeof(ast_node *), 1);
    if (println_params) {
        println_params[0] = str_type;
    }

    analyzer.builtin_procedures[1] = {.name = "println",
                                      .type = builtin_procedure_type::println,
                                      .param_count = 1,
                                      .param_types = println_params,
                                      .return_type = void_type};
}

builtin_procedure *lookup_builtin_procedure(semantic_analyzer &analyzer, const char *name, u32 name_len) {
    for (u32 i = 0; i < analyzer.builtin_procedure_count; i++) {
        builtin_procedure &proc = analyzer.builtin_procedures[i];
        if (strlen(proc.name) == name_len && strncmp(proc.name, name, name_len) == 0) {
            return &proc;
        }
    }
    return nullptr;
}

bool is_builtin_procedure_call(semantic_analyzer &analyzer, const char *name, u32 name_len) {
    return lookup_builtin_procedure(analyzer, name, name_len) != nullptr;
}

// Add procedure call analysis:
bool analyze_call_expression(semantic_analyzer &analyzer, ast_expr_call *call) {
    // Analyze the procedure expression
    if (!analyze_expression(analyzer, call->procedure)) {
        return false;
    }

    // Analyze all arguments
    for (u32 i = 0; i < call->argument_count; i++) {
        if (!analyze_expression(analyzer, call->arguments[i])) {
            return false;
        }
    }

    // Check if this is a builtin procedure call
    if (call->procedure->type == ast_node_type::EXPR_IDENTIFIER) {
        ast_expr_identifier *proc_name = (ast_expr_identifier *)call->procedure;

        builtin_procedure *builtin = lookup_builtin_procedure(analyzer, proc_name->name, proc_name->name_len);
        if (builtin) {
            // Validate builtin procedure call
            if (call->argument_count != builtin->param_count) {
                semantic_error_at_node(analyzer, "Wrong number of arguments for builtin procedure", (ast_node *)call);
                return false;
            }

            // TODO: Type check arguments
            return true;
        }

        // Check if it's a user-defined procedure
        symbol *proc_symbol = symbol_table_lookup(analyzer.current_scope, proc_name->name, proc_name->name_len);
        if (!proc_symbol) {
            semantic_error_at_node(analyzer, "Undefined procedure", call->procedure);
            return false;
        }

        if (proc_symbol->type != symbol_type::PROCEDURE) {
            semantic_error_at_node(analyzer, "Not a procedure", call->procedure);
            return false;
        }

        // Validate parameter count
        if (call->argument_count != proc_symbol->info.procedure.parameter_count) {
            semantic_error_at_node(analyzer, "Wrong number of arguments", (ast_node *)call);
            return false;
        }

        // TODO: Type check arguments against parameters
    }

    return true;
}
