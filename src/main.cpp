#include "compiler.hpp"
#include "logger.hpp"
#include <cstdio>

i32 main(i32 argc, char **argv) {
    // Parse command line arguments
    compiler_options options = parse_command_line(argc, argv);

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
