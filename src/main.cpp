// #include "compiler.hpp"
#include "allocators.hpp"
#include "collections/pool.hpp"
#include "defines.hpp"
#include "logger.hpp"
#include <cstdio>

// void debug_compiler_options(compiler_options &options);

block_allocator root_allocator, sub_allocator;
arena_allocator lil_allocators[5], sub_lil_allocators[10];

void stats();

struct thing {
    u64 a, b, c, d;
};

i32 main() {
    root_allocator.init_as_root(kilobytes(4), kilobytes(32));
    sub_allocator.init(root_allocator, kilobytes(4), kilobytes(2), 2.0f);

    stats();
    info();

    for (u32 i = 0; i < 5; i++) {
        auto &a = lil_allocators[i];
        a.init(sub_allocator, kilobytes(1));
    }

    for (u32 i = 0; i < 10; i++) {
        int parent_id = i / 2;
        auto &a = sub_lil_allocators[i];
        a.init(lil_allocators[parent_id], kilobytes(1) / 2);
    }

    pool<thing> things;
    things.init(sub_allocator, 16);
    for (u32 i = 0; i < 16; i++) {
        things.push({});
    }

    info("pool: %d/%d", things.size, things.capacity);
    things.push({});
    info("pool: %d/%d", things.size, things.capacity);

    for (u32 i = 0; i < 10; i++) {
        sub_lil_allocators[i].deinit();
    }

    for (u32 i = 0; i < 5; i++) {
        lil_allocators[i].deinit();
    }

    sub_allocator.deinit();
    root_allocator.deinit();

    stats();

    return 0;
}

void stats() {
    root_allocator.stats();
    sub_allocator.stats();

    for (u32 i = 0; i < 5; i++) {
        lil_allocators[i].stats();
    }

    for (u32 i = 0; i < 10; i++) {
        sub_lil_allocators[i].stats();
    }
}

// i32 main(i32 argc, char **argv) {
//     // Parse command line arguments
//     compiler_options options = parse_command_line(argc, argv);
//     debug_compiler_options(options);

//     // Create compiler instance
//     struct compiler compiler;

//     // Compile the program
//     compiler_result result = compile(compiler, options);

//     if (!result.success) {
//         if (result.error_row > 0) {
//             error("❌ %s at line %u, column %u", result.error_message.c_str(), result.error_row, result.error_col);
//         } else {
//             error("❌ %s", result.error_message.c_str());
//         }
//         return 1;
//     }

//     // Show success message
//     if (options.verbose) {
//         info("🎉 Compilation completed successfully!");

//         if (options.run_after_compile) {
//             info("Program exited with code: %d", result.exit_code);
//         }
//     }

//     return 0;
// }

// void debug_compiler_options(compiler_options &opts) {
//     printf("Options:\n");
//     printf("  In file: %s\n", opts.input_file.c_str());
//     printf("  Out file: %s\n", opts.output_file.c_str());
//     printf("  Exe: %s\n", opts.executable_name.c_str());
//     printf("  Verbose: %s\n", opts.verbose ? "true" : "false");
//     printf("  Debug Tokens: %s\n", opts.debug_tokens ? "true" : "false");
//     printf("  Debug Ast: %s\n", opts.debug_ast ? "true" : "false");
//     printf("  Debug Symbols: %s\n", opts.debug_symbols ? "true" : "false");
//     printf("  Run After Compile: %s\n", opts.run_after_compile ? "true" : "false");
//     printf("  Keep Assembly: %s\n", opts.keep_assembly ? "true" : "false");
// }
