#pragma once

#include "defines.hpp"
#include "lexer.hpp"
#include "memory.hpp"
#include "parser.hpp"
#include "semantic.hpp"
#include "x86_generator.hpp"
#include <string>

struct compiler_options {
    std::string input_file;
    std::string output_file = "output.s";
    std::string executable_name = "program";
    bool verbose = false;
    bool debug_tokens = false;
    bool debug_ast = false;
    bool debug_symbols = false;
    bool run_after_compile = false;
    bool keep_assembly = false;
};

struct compiler_result {
    bool success = false;
    std::string error_message;
    u32 error_row = 0, error_col = 0;
    i32 exit_code = -1;
};

struct compiler {
    arena memory = {};
    struct lexer lexer = {};
    struct parser parser = {};
    semantic_analyzer analyzer = {};
    x86_generator generator = {};
    ast_node *program = nullptr;
    compiler_options current_options = {};
};

compiler_result compile(struct compiler &compiler, const compiler_options &options);
compiler_result compile_from_string(struct compiler &compiler, const char *source_code,
                                    const compiler_options &options);
compiler_options parse_command_line(int argc, char **argv);
