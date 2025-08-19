#include "compiler.hpp"
#include "allocators.hpp"
#include "ast.hpp"
#include "lexer.hpp"
#include "logger.hpp"
#include "parser.hpp"
#include "semantic.hpp"
#include "x86_generator.hpp"

#include <cstdlib>
#include <fstream>
#include <sstream>

static bool lexical_analysis(compiler &c, const char *source);
static bool syntax_analysis(compiler &c);
static bool semantic_analysis(compiler &c);
static bool code_generation(compiler &c, const std::string &output_file);
static bool assemble_and_link(compiler &c, const std::string &asm_file, const std::string &executable);
static void show_assembly_if_requested(compiler &c, const std::string &asm_file);
static i32 run_program(compiler &c, const std::string &executable);
static void debug_tokens(compiler &c);
static void debug_ast(compiler &c);
static void debug_symbols(compiler &c);
static std::string read_file(compiler &c, const std::string &filename);
static void cleanup(compiler &c);

static void print_indent(int indent);
static void print_ast_node(ast_node *node, int indent);
static void print_symbol_table(symbol_table *table, int indent);
const char *ast_node_type_to_string(ast_node_type type);
const char *binary_op_to_string(binary_op op);
const char *builtin_type_to_string_updated(builtin_type type);
const char *literal_type_to_string(literal_type type);
const char *unary_op_to_string(unary_op op);

compiler_result compile(compiler &c, const compiler_options &options) {
    c.current_options = options;

    info("Starting compilation of '%s'", options.input_file.c_str());

    // Read source file
    std::string source = read_file(c, options.input_file);
    if (source.empty()) {
        return {false, "Failed to read input file: " + options.input_file, 0, 0};
    }

    return compile_from_string(c, source.c_str(), options);
}

compiler_result compile_from_string(compiler &c, const char *source_code, const compiler_options &options) {
    cleanup(c); // Ensure clean state
    c.current_options = options;

    // Initialize memory
    if (!c.allocator.init(megabytes(1))) {
        return {false, "Failed to initialize memory arena", 0, 0};
    }

    // Phase 1: Lexical Analysis
    if (!lexical_analysis(c, source_code)) {
        return {false, "Lexical analysis failed", 0, 0};
    }

    // Phase 2: Syntax Analysis
    if (!syntax_analysis(c)) {
        std::string msg = std::string("Parse error: ") + c.parser.error.message;
        return {false, msg, c.parser.error.row, c.parser.error.column};
    }

    // Phase 2.5: Semantic Analysis
    if (!semantic_analysis(c)) {
        std::string msg = std::string("Semantic error: ") + c.analyzer.error_message;
        return {false, msg, c.analyzer.error_row, c.analyzer.error_column};
    }

    // Phase 3: Code Generation
    if (!code_generation(c, options.output_file)) {
        return {false, "Code generation failed", 0, 0};
    }

    // Phase 4: Assembly and Linking
    if (!assemble_and_link(c, options.output_file, options.executable_name)) {
        return {false, "Assembly and linking failed", 0, 0};
    }

    // Phase 5: Run if requested
    int exit_code = 0;
    if (options.run_after_compile) {
        exit_code = run_program(c, options.executable_name);
    }

    info("✅ Compilation completed successfully!");
    return {true, "", 0, 0, exit_code};
}

static bool lexical_analysis(compiler &c, const char *source) {
    if (c.current_options.verbose) info("📝 Phase 1: Lexical Analysis");

    if (!lexer_load(c.lexer, source)) {
        error("Failed to load source code");
        return false;
    }

    if (c.current_options.debug_tokens) {
        debug_tokens(c);
    }

    if (c.current_options.verbose) info("✅ Lexical analysis completed");
    return true;
}

static bool syntax_analysis(compiler &c) {
    if (c.current_options.verbose) info("🔍 Phase 2: Syntax Analysis");

    if (!parser_init(c.parser, c.lexer, c.allocator)) {
        error("Failed to initialize parser");
        return false;
    }

    c.program = parse_program(c.parser);

    if (c.parser.has_error || !c.program) {
        return false;
    }

    if (c.current_options.debug_ast) {
        debug_ast(c);
    }

    if (c.current_options.verbose) {
        info("✅ Syntax analysis completed");
        stats(c.allocator);
    }

    return true;
}

static bool semantic_analysis(compiler &c) {
    if (c.current_options.verbose) info("🧠 Phase 2.5: Semantic Analysis");

    if (!semantic_analyzer_init(c.analyzer, c.allocator)) {
        error("Failed to initialize semantic analyzer");
        return false;
    }

    if (!analyze(c.analyzer, c.program)) {
        return false;
    }

    if (c.current_options.debug_symbols) {
        debug_symbols(c);
    }

    if (c.current_options.verbose) info("✅ Semantic analysis completed");
    return true;
}

static bool code_generation(compiler &c, const std::string &output_file) {
    if (c.current_options.verbose) info("⚙️  Phase 3: Code Generation");

    printf("[DEBUG] Creating assembly file: %s\n", output_file.c_str());

    FILE *asm_file = fopen(output_file.c_str(), "w");
    if (!asm_file) {
        error("Failed to create output file: %s", output_file.c_str());
        return false;
    }

    if (!x86_gen_init(c.generator, c.allocator, asm_file)) {
        error("Failed to initialize code generator");
        fclose(asm_file);
        return false;
    }

    bool success = generate_x86(c.generator, c.program);
    fclose(asm_file);

    if (!success) {
        error("Code generation failed");
        return false;
    }

    // Check if file exists and show its size
    std::ifstream check_file(output_file);
    if (check_file.good()) {
        check_file.seekg(0, std::ios::end);
        std::streampos size = check_file.tellg();
        printf("[DEBUG] Assembly file created successfully: %s (%ld bytes)\n", output_file.c_str(), (long)size);
        check_file.close();
    } else {
        printf("[DEBUG] WARNING: Assembly file was not created or is not readable\n");
    }

    if (c.current_options.verbose) {
        info("✅ Assembly code generated in %s", output_file.c_str());
    }

    // Show assembly content if requested
    show_assembly_if_requested(c, output_file);

    return true;
}

static bool assemble_and_link(compiler &c, const std::string &asm_file, const std::string &executable) {
    if (c.current_options.verbose) info("🔧 Phase 4: Assembly and Linking");

    printf("[DEBUG] Assembly file before linking: %s\n", asm_file.c_str());
    printf("[DEBUG] Keep assembly flag: %s\n", c.current_options.keep_assembly ? "true" : "false");

    // Check if assembly file exists before linking
    std::ifstream check_asm(asm_file);
    if (!check_asm.good()) {
        error("Assembly file does not exist: %s", asm_file.c_str());
        return false;
    }
    check_asm.close();

    // Use ld directly to avoid libc linking, or use gcc with -nostdlib
    // Option 1: Use ld directly (more control, but more complex)
    // std::string cmd = "ld -o " + executable + " " + asm_file;

    // Option 2: Use gcc with -nostdlib (simpler, still avoids libc)
    std::string cmd = "gcc -nostdlib -static -o " + executable + " " + asm_file + " 2>/dev/null";

    printf("[DEBUG] Running: %s\n", cmd.c_str());

    int result = system(cmd.c_str());

    if (result == 0) {
        if (c.current_options.verbose) {
            info("✅ Successfully compiled to executable '%s'", executable.c_str());

            // Show binary size
            std::string size_cmd = "ls -lh " + executable + " | awk '{print $5}'";
            printf("[INFO] Binary size: ");
            system(size_cmd.c_str());
        }

        // Only clean up assembly file if NOT keeping it
        if (!c.current_options.keep_assembly) {
            if (c.current_options.verbose) {
                info("🗑️ Removing assembly file %s", asm_file.c_str());
            }
            printf("[DEBUG] Removing assembly file: %s\n", asm_file.c_str());
            std::remove(asm_file.c_str());
        } else {
            if (c.current_options.verbose) {
                info("📄 Keeping assembly file: %s", asm_file.c_str());
            }
            printf("[DEBUG] Keeping assembly file: %s\n", asm_file.c_str());
        }

        return true;
    } else {
        warn("❌ Assembly and linking failed");
        info("💡 You can inspect %s to see the generated code", asm_file.c_str());
        return false;
    }
}

static void show_assembly_if_requested(compiler &c, const std::string &asm_file) {
    if (c.current_options.verbose && c.current_options.keep_assembly) {
        info("📄 Generated Assembly (%s):", asm_file.c_str());
        printf("=====================================\n");
        std::string cmd = "cat " + asm_file;
        system(cmd.c_str());
        printf("=====================================\n");
    }
}

static i32 run_program(compiler &c, const std::string &executable) {
    if (c.current_options.verbose) info("🏃 Phase 5: Running the program...");

    std::string cmd = "./" + executable;
    i32 run_result = system(cmd.c_str());
    i32 exit_code = WEXITSTATUS(run_result);

    if (c.current_options.verbose) {
        info("✅ Program executed with exit code: %d", exit_code);
    }

    return exit_code;
}

static void debug_tokens(compiler &c) {
    info("🔍 Debug: Lexing all tokens...");

    lexer debug_lexer = c.lexer;
    i32 token_count = 0;

    while (true) {
        token tok = lexer_next_token(debug_lexer);
        printf("Token %d: %s at line %u, col %u\n", token_count++, token_to_string(tok).c_str(), tok.row, tok.column);

        if (tok.type == token_type::END_OF_FILE) break;
        if (token_count > 100) break; // Safety limit
    }

    info("🔍 Debug: Done lexing tokens");
}

static void debug_ast(compiler &c) {
    info("🌳 AST Structure:");
    // TODO(xiu): adjust indentation depth dynamically
    print_ast_node(c.program, 0);
}

static void debug_symbols(compiler &c) {
    info("📋 Symbol Table:");
    // TODO(xiu): adjust indentation depth dynamically
    print_symbol_table(c.analyzer.global_scope, 0);
}

static std::string read_file(compiler &c, const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        error("Could not open file: %s", filename.c_str());
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

static void cleanup(compiler &c) {
    if (c.allocator.memory) {
        x86_gen_deinit(c.generator);
        semantic_analyzer_deinit(c.analyzer);
        parser_deinit(c.parser);
        deinit(c.allocator);
    }

    // compiler_options saved_options = c.current_options;
    c = {};
    // c.current_options = saved_options;
}

// =============================================================================
// COMMAND LINE PARSING
// =============================================================================

void print_usage(const char *program_name) {
    printf("Usage: %s [options] <input_file>\n", program_name);
    printf("\nOptions:\n");
    printf("  -o <file>     Output assembly file (default: output.s)\n");
    printf("  -e <name>     Executable name (default: program)\n");
    printf("  -v, --verbose Verbose output\n");
    printf("  -r, --run     Run the program after compilation\n");
    printf("  -k, --keep    Keep assembly file after compilation\n");
    printf("  --debug-tokens Show token stream\n");
    printf("  --debug-ast   Show AST structure\n");
    printf("  --debug-symbols Show symbol table\n");
    printf("  -h, --help    Show this help message\n");
    printf("\nExamples:\n");
    printf("  %s hello.lang\n", program_name);
    printf("  %s -v -r -o hello.s hello.lang\n", program_name);
    printf("  %s --debug-ast hello.lang\n", program_name);
}

compiler_options parse_command_line(int argc, char **argv) {
    compiler_options options;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            print_usage(argv[0]);
            exit(0);
        } else if (arg == "-v" || arg == "--verbose") {
            options.verbose = true;
        } else if (arg == "-r" || arg == "--run") {
            options.run_after_compile = true;
        } else if (arg == "-k" || arg == "--keep") {
            options.keep_assembly = true;
        } else if (arg == "--debug-tokens") {
            options.debug_tokens = true;
        } else if (arg == "--debug-ast") {
            options.debug_ast = true;
        } else if (arg == "--debug-symbols") {
            options.debug_symbols = true;
        } else if (arg == "-o" && i + 1 < argc) {
            options.output_file = argv[++i];
        } else if (arg == "-e" && i + 1 < argc) {
            options.executable_name = argv[++i];
        } else if (arg[0] != '-') {
            // Input file
            if (options.input_file.empty()) {
                options.input_file = arg;
            } else {
                error("Multiple input files not supported: %s", arg.c_str());
                exit(1);
            }
        } else {
            error("Unknown option: %s", arg.c_str());
            print_usage(argv[0]);
            exit(1);
        }
    }

    if (options.input_file.empty()) {
        error("No input file specified");
        print_usage(argv[0]);
        exit(1);
    }

    return options;
}

// =============================================================================
// UTILITY
// =============================================================================

void print_indent(int indent) {
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }
}

static void print_ast_node(ast_node *node, int indent) {
    if (!node) {
        print_indent(indent);
        printf("(null)\n");
        return;
    }

    print_indent(indent);
    printf("%s", ast_node_type_to_string(node->type));

    switch (node->type) {
    case ast_node_type::TRANSLATION_UNIT: {
        ast_translation_unit *tu = (ast_translation_unit *)node;
        printf(" (%u declarations)\n", tu->declaration_count);
        for (u32 i = 0; i < tu->declaration_count; i++) {
            print_ast_node(tu->declarations[i], indent + 1);
        }
        break;
    }

    case ast_node_type::DECL_PROCEDURE: {
        ast_decl_procedure *proc = (ast_decl_procedure *)node;
        printf(" '%.*s' (%u params)", proc->name_len, proc->name, proc->parameter_count);
        if (proc->has_explicit_return_type) {
            printf(" -> explicit return type\n");
        } else {
            printf(" -> inferred return type\n");
        }

        if (proc->return_type) {
            print_indent(indent + 1);
            printf("return_type:\n");
            print_ast_node(proc->return_type, indent + 2);
        }

        if (proc->body) {
            print_indent(indent + 1);
            printf("body:\n");
            print_ast_node(proc->body, indent + 2);
        }
        break;
    }

    case ast_node_type::DECL_VARIABLE: {
        ast_decl_variable *var = (ast_decl_variable *)node;
        printf(" '%.*s'", var->name_len, var->name);
        if (var->has_explicit_type) {
            printf(" (explicit type)\n");
        } else {
            printf(" (inferred type)\n");
        }

        if (var->var_type) {
            print_indent(indent + 1);
            printf("type:\n");
            print_ast_node(var->var_type, indent + 2);
        }

        if (var->initializer) {
            print_indent(indent + 1);
            printf("initializer:\n");
            print_ast_node(var->initializer, indent + 2);
        }
        break;
    }

    case ast_node_type::DECL_STRUCT: {
        ast_decl_struct *struct_decl = (ast_decl_struct *)node;
        printf(" '%.*s' (%u fields)\n", struct_decl->name_len, struct_decl->name, struct_decl->field_count);
        for (u32 i = 0; i < struct_decl->field_count; i++) {
            print_ast_node(struct_decl->fields[i], indent + 1);
        }
        break;
    }

    case ast_node_type::DECL_UNION: {
        ast_decl_union *union_decl = (ast_decl_union *)node;
        printf(" '%.*s' (%u fields)\n", union_decl->name_len, union_decl->name, union_decl->field_count);
        for (u32 i = 0; i < union_decl->field_count; i++) {
            print_ast_node(union_decl->fields[i], indent + 1);
        }
        break;
    }

    case ast_node_type::DECL_ENUM: {
        ast_decl_enum *enum_decl = (ast_decl_enum *)node;
        printf(" '%.*s' (%u values)\n", enum_decl->name_len, enum_decl->name, enum_decl->value_count);
        for (u32 i = 0; i < enum_decl->value_count; i++) {
            print_ast_node(enum_decl->values[i], indent + 1);
        }
        break;
    }

    case ast_node_type::DECL_TYPEDEF: {
        ast_decl_typedef *typedef_decl = (ast_decl_typedef *)node;
        printf(" '%.*s'\n", typedef_decl->name_len, typedef_decl->name);
        if (typedef_decl->type) {
            print_indent(indent + 1);
            printf("type:\n");
            print_ast_node(typedef_decl->type, indent + 2);
        }
        break;
    }

    case ast_node_type::STMT_EXPRESSION: {
        ast_stmt_expression *expr_stmt = (ast_stmt_expression *)node;
        printf("\n");
        if (expr_stmt->expression) {
            print_ast_node(expr_stmt->expression, indent + 1);
        }
        break;
    }

    case ast_node_type::STMT_COMPOUND: {
        ast_stmt_compound *compound = (ast_stmt_compound *)node;
        printf(" (%u statements)\n", compound->statement_count);
        for (u32 i = 0; i < compound->statement_count; i++) {
            print_ast_node(compound->statements[i], indent + 1);
        }
        break;
    }

    case ast_node_type::STMT_IF: {
        ast_stmt_if *if_stmt = (ast_stmt_if *)node;
        printf("\n");
        print_indent(indent + 1);
        printf("condition:\n");
        print_ast_node(if_stmt->condition, indent + 2);
        print_indent(indent + 1);
        printf("then:\n");
        print_ast_node(if_stmt->then_stmt, indent + 2);
        if (if_stmt->else_stmt) {
            print_indent(indent + 1);
            printf("else:\n");
            print_ast_node(if_stmt->else_stmt, indent + 2);
        }
        break;
    }

    case ast_node_type::STMT_WHILE: {
        ast_stmt_while *while_stmt = (ast_stmt_while *)node;
        printf("\n");
        print_indent(indent + 1);
        printf("condition:\n");
        print_ast_node(while_stmt->condition, indent + 2);
        print_indent(indent + 1);
        printf("body:\n");
        print_ast_node(while_stmt->body, indent + 2);
        break;
    }

    case ast_node_type::STMT_FOR: {
        ast_stmt_for *for_stmt = (ast_stmt_for *)node;
        printf("\n");
        if (for_stmt->init) {
            print_indent(indent + 1);
            printf("init:\n");
            print_ast_node(for_stmt->init, indent + 2);
        }
        if (for_stmt->condition) {
            print_indent(indent + 1);
            printf("condition:\n");
            print_ast_node(for_stmt->condition, indent + 2);
        }
        if (for_stmt->update) {
            print_indent(indent + 1);
            printf("increment:\n");
            print_ast_node(for_stmt->update, indent + 2);
        }
        print_indent(indent + 1);
        printf("body:\n");
        print_ast_node(for_stmt->body, indent + 2);
        break;
    }

    case ast_node_type::STMT_RETURN: {
        ast_stmt_return *ret = (ast_stmt_return *)node;
        printf("\n");
        if (ret->value) {
            print_indent(indent + 1);
            printf("value:\n");
            print_ast_node(ret->value, indent + 2);
        }
        break;
    }

    case ast_node_type::STMT_BREAK: {
        printf("\n");
        break;
    }

    case ast_node_type::STMT_CONTINUE: {
        printf("\n");
        break;
    }

    case ast_node_type::STMT_SWITCH: {
        ast_stmt_switch *switch_stmt = (ast_stmt_switch *)node;
        printf("\n");
        print_indent(indent + 1);
        printf("expression:\n");
        print_ast_node(switch_stmt->expression, indent + 2);
        print_indent(indent + 1);
        printf("body:\n");
        print_ast_node(switch_stmt->body, indent + 2);
        break;
    }

    case ast_node_type::STMT_CASE: {
        ast_stmt_case *case_stmt = (ast_stmt_case *)node;
        printf("\n");
        print_indent(indent + 1);
        printf("value:\n");
        print_ast_node(case_stmt->value, indent + 2);
        if (case_stmt->statement) {
            print_indent(indent + 1);
            printf("statement:\n");
            print_ast_node(case_stmt->statement, indent + 2);
        }
        break;
    }

    case ast_node_type::STMT_DEFAULT: {
        ast_stmt_default *default_stmt = (ast_stmt_default *)node;
        printf("\n");
        if (default_stmt->statement) {
            print_indent(indent + 1);
            printf("statement:\n");
            print_ast_node(default_stmt->statement, indent + 2);
        }
        break;
    }

    case ast_node_type::STMT_GOTO: {
        ast_stmt_goto *goto_stmt = (ast_stmt_goto *)node;
        printf(" '%.*s'\n", goto_stmt->label_len, goto_stmt->label);
        break;
    }

    case ast_node_type::STMT_LABEL: {
        ast_stmt_label *label_stmt = (ast_stmt_label *)node;
        printf(" '%.*s'\n", label_stmt->label_len, label_stmt->label);
        if (label_stmt->statement) {
            print_ast_node(label_stmt->statement, indent + 1);
        }
        break;
    }

    case ast_node_type::EXPR_BINARY: {
        ast_expr_binary *binary = (ast_expr_binary *)node;
        printf(" %s\n", binary_op_to_string(binary->op));
        print_indent(indent + 1);
        printf("left:\n");
        print_ast_node(binary->left, indent + 2);
        print_indent(indent + 1);
        printf("right:\n");
        print_ast_node(binary->right, indent + 2);
        break;
    }

    case ast_node_type::EXPR_UNARY: {
        ast_expr_unary *unary = (ast_expr_unary *)node;
        printf(" %s\n", unary_op_to_string(unary->op));
        print_indent(indent + 1);
        printf("operand:\n");
        print_ast_node(unary->operand, indent + 2);
        break;
    }

    case ast_node_type::EXPR_CALL: {
        ast_expr_call *call = (ast_expr_call *)node;
        printf(" (%u args)\n", call->argument_count);
        print_indent(indent + 1);
        printf("procedure:\n");
        print_ast_node(call->procedure, indent + 2);
        if (call->argument_count > 0) {
            print_indent(indent + 1);
            printf("arguments:\n");
            for (u32 i = 0; i < call->argument_count; i++) {
                print_ast_node(call->arguments[i], indent + 2);
            }
        }
        break;
    }

    case ast_node_type::EXPR_MEMBER: {
        ast_expr_member *member = (ast_expr_member *)node;
        printf(" '%.*s'\n", member->member_len, member->member);
        print_indent(indent + 1);
        printf("object:\n");
        print_ast_node(member->object, indent + 2);
        break;
    }

    case ast_node_type::EXPR_INDEX: {
        ast_expr_index *index = (ast_expr_index *)node;
        printf("\n");
        print_indent(indent + 1);
        printf("array:\n");
        print_ast_node(index->array, indent + 2);
        print_indent(indent + 1);
        printf("index:\n");
        print_ast_node(index->index, indent + 2);
        break;
    }

    case ast_node_type::EXPR_CAST: {
        ast_expr_cast *cast = (ast_expr_cast *)node;
        printf("\n");
        print_indent(indent + 1);
        printf("type:\n");
        print_ast_node(cast->target_type, indent + 2);
        print_indent(indent + 1);
        printf("expression:\n");
        print_ast_node(cast->expression, indent + 2);
        break;
    }

    case ast_node_type::EXPR_SIZEOF: {
        ast_expr_sizeof *sizeof_expr = (ast_expr_sizeof *)node;
        printf("\n");
        if (sizeof_expr->type) {
            print_indent(indent + 1);
            printf("type:\n");
            print_ast_node(sizeof_expr->type, indent + 2);
        }
        if (sizeof_expr->expression) {
            print_indent(indent + 1);
            printf("expression:\n");
            print_ast_node(sizeof_expr->expression, indent + 2);
        }
        break;
    }

    case ast_node_type::EXPR_LITERAL: {
        ast_expr_literal *lit = (ast_expr_literal *)node;
        printf(" %s: '%.*s'\n", literal_type_to_string(lit->type), lit->value_len, lit->value);
        break;
    }

    case ast_node_type::EXPR_IDENTIFIER: {
        ast_expr_identifier *ident = (ast_expr_identifier *)node;
        printf(" '%.*s'\n", ident->name_len, ident->name);
        break;
    }

    case ast_node_type::TYPE_BUILTIN: {
        ast_type_builtin *builtin = (ast_type_builtin *)node;
        printf(" %s\n", builtin_type_to_string_updated(builtin->type));
        break;
    }

    case ast_node_type::TYPE_POINTER: {
        ast_type_pointer *pointer = (ast_type_pointer *)node;
        printf("\n");
        print_indent(indent + 1);
        printf("pointee:\n");
        print_ast_node(pointer->to, indent + 2);
        break;
    }

    case ast_node_type::TYPE_ARRAY: {
        ast_type_array *array = (ast_type_array *)node;
        printf("\n");
        print_indent(indent + 1);
        printf("element_type:\n");
        print_ast_node(array->element_type, indent + 2);
        if (array->size) {
            print_indent(indent + 1);
            printf("size:\n");
            print_ast_node(array->size, indent + 2);
        }
        break;
    }

    case ast_node_type::TYPE_PROCEDURE: {
        ast_type_procedure *proc_type = (ast_type_procedure *)node;
        printf(" (%u params)\n", proc_type->parameter_count);
        if (proc_type->return_type) {
            print_indent(indent + 1);
            printf("return_type:\n");
            print_ast_node(proc_type->return_type, indent + 2);
        }
        if (proc_type->parameter_count > 0) {
            print_indent(indent + 1);
            printf("parameters:\n");
            for (u32 i = 0; i < proc_type->parameter_count; i++) {
                print_ast_node(proc_type->parameter_types[i], indent + 2);
            }
        }
        break;
    }

    case ast_node_type::TYPE_STRUCT: {
        ast_type_struct *struct_type = (ast_type_struct *)node;
        printf(" '%.*s'\n", struct_type->name_len, struct_type->name);
        break;
    }

    case ast_node_type::TYPE_UNION: {
        ast_type_union *union_type = (ast_type_union *)node;
        printf(" '%.*s'\n", union_type->name_len, union_type->name);
        break;
    }

    case ast_node_type::TYPE_ENUM: {
        ast_type_enum *enum_type = (ast_type_enum *)node;
        printf(" '%.*s'\n", enum_type->name_len, enum_type->name);
        break;
    }

    case ast_node_type::TYPE_INFERRED: {
        printf("\n");
        break;
    }

    case ast_node_type::DECL_MODULE: {
        ast_decl_module *module = (ast_decl_module *)node;
        printf(" '%.*s' (%u declarations)\n", module->name_len, module->name, module->declaration_count);
        for (u32 i = 0; i < module->declaration_count; i++) {
            print_ast_node(module->declarations[i], indent + 1);
        }
        break;
    }

    case ast_node_type::DECL_USE: {
        ast_decl_use *use_decl = (ast_decl_use *)node;
        printf(" (%u items)\n", use_decl->item_count);
        for (u32 i = 0; i < use_decl->item_count; i++) {
            // TODO(xiu): Print individual use items
            printf("  use item %u\n", i);
        }
        break;
    }

    case ast_node_type::EXPR_MEMBER_ACCESS: {
        ast_expr_member_access *access = (ast_expr_member_access *)node;
        printf(" '%.*s'\n", access->member_len, access->member);
        print_indent(indent + 1);
        printf("object:\n");
        print_ast_node(access->object, indent + 2);
        break;
    }

    default:
        printf(" (printer not implemented for type %d)\n", (int)node->type);
        break;
    }
}

static void print_symbol_table(symbol_table *table, int indent) {
    if (!table) return;

    for (int i = 0; i < indent; i++)
        printf("  ");
    printf("Scope (depth %u, %u symbols):\n", table->scope_depth, table->count);

    for (u32 i = 0; i < table->count; i++) {
        symbol &sym = table->symbols[i];
        for (int j = 0; j < indent + 1; j++)
            printf("  ");
        printf("- %s '%.*s'", symbol_type_to_string(sym.type), sym.name_len, sym.name);

        if (sym.type == symbol_type::VARIABLE) {
            printf(" (offset: %d, global: %s, initialized: %s)", sym.info.variable.stack_offset,
                   sym.info.variable.is_global ? "yes" : "no", sym.info.variable.is_initialized ? "yes" : "no");
        } else if (sym.type == symbol_type::PROCEDURE) {
            printf(" (%u params, has_body: %s)", sym.info.procedure.parameter_count,
                   sym.info.procedure.has_body ? "yes" : "no");
        }
        printf("\n");
    }
}

const char *ast_node_type_to_string(ast_node_type type) {
    switch (type) {
    case ast_node_type::TRANSLATION_UNIT:
        return "TranslationUnit";
    case ast_node_type::DECL_PROCEDURE:
        return "ProcedureDecl";
    case ast_node_type::DECL_VARIABLE:
        return "VariableDecl";
    case ast_node_type::STMT_COMPOUND:
        return "CompoundStmt";
    case ast_node_type::STMT_IF:
        return "IfStmt";
    case ast_node_type::STMT_RETURN:
        return "ReturnStmt";
    case ast_node_type::EXPR_BINARY:
        return "BinaryExpr";
    case ast_node_type::EXPR_CALL:
        return "CallExpr";
    case ast_node_type::EXPR_LITERAL:
        return "Literal";
    case ast_node_type::EXPR_IDENTIFIER:
        return "Identifier";
    case ast_node_type::TYPE_BUILTIN:
        return "BuiltinType";
    case ast_node_type::DECL_MODULE:
        return "ModuleDecl";
    case ast_node_type::DECL_USE:
        return "UseDecl";
    case ast_node_type::EXPR_MEMBER_ACCESS:
        return "MemberAccess";
    default:
        return "Unknown";
    }
}

const char *binary_op_to_string(binary_op op) {
    switch (op) {
    case binary_op::ADD:
        return "+";
    case binary_op::SUB:
        return "-";
    case binary_op::MUL:
        return "*";
    case binary_op::DIV:
        return "/";
    case binary_op::EQ:
        return "==";
    case binary_op::NE:
        return "!=";
    default:
        return "?";
    }
}

const char *builtin_type_to_string_updated(builtin_type type) {
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
    default:
        return "unknown_type";
    }
}

const char *literal_type_to_string(literal_type type) {
    switch (type) {
    case literal_type::INTEGER:
        return "int";
    case literal_type::FLOAT:
        return "float";
    case literal_type::CHARACTER:
        return "char";
    case literal_type::STRING:
        return "string";
    case literal_type::BOOLEAN:
        return "bool";
    default:
        return "unknown";
    }
}

const char *unary_op_to_string(unary_op op) {
    switch (op) {
    case unary_op::PLUS:
        return "+";
    case unary_op::MINUS:
        return "-";
    case unary_op::NOT:
        return "!";
    default:
        return "?";
    }
}
