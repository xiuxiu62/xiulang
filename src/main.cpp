#include "compiler.hpp"
#include "logger.hpp"
#include <cstdio>

void debug_compiler_options(compiler_options &options);

i32 main(i32 argc, char **argv) {
    // Parse command line arguments
    compiler_options options = parse_command_line(argc, argv);
    debug_compiler_options(options);

    // Create compiler instance
    struct compiler compiler;

    // Compile the program
    compiler_result result = compile(compiler, options);

    if (!result.success) {
        if (result.error_row > 0) {
            error("❌ %s at line %u, column %u", result.error_message.c_str(), result.error_row, result.error_col);
        } else {
            error("❌ %s", result.error_message.c_str());
        }
        return 1;
    }

    // Show success message
    if (options.verbose) {
        info("🎉 Compilation completed successfully!");

        if (options.run_after_compile) {
            info("Program exited with code: %d", result.exit_code);
        }
    }

    return 0;
}

void debug_compiler_options(compiler_options &opts) {
    printf("Options:\n");
    printf("  In file: %s\n", opts.input_file.c_str());
    printf("  Out file: %s\n", opts.output_file.c_str());
    printf("  Exe: %s\n", opts.executable_name.c_str());
    printf("  Verbose: %s\n", opts.verbose ? "true" : "false");
    printf("  Debug Tokens: %s\n", opts.debug_tokens ? "true" : "false");
    printf("  Debug Ast: %s\n", opts.debug_ast ? "true" : "false");
    printf("  Debug Symbols: %s\n", opts.debug_symbols ? "true" : "false");
    printf("  Run After Compile: %s\n", opts.run_after_compile ? "true" : "false");
    printf("  Keep Assembly: %s\n", opts.keep_assembly ? "true" : "false");
}
