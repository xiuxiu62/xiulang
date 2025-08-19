#pragma once

#include "ast.hpp"
#include "memory.hpp"
#include <cstdio>

enum class x86_linux_register {
    RAX,
    RBX,
    RCX,
    RDX,
    RSI,
    RDI,
    RBP,
    RSP,
    R8,
    R9,
    R10,
    R11,
    R12,
    R13,
    R14,
    R15,

    // 32-bit versions
    EAX,
    EBX,
    ECX,
    EDX,
    ESI,
    EDI,
    EBP,
    ESP,

    // For procedure arguments (System V ABI)
    ARG1 = RDI, // First argument
    ARG2 = RSI, // Second argument
    ARG3 = RDX, // Third argument
    ARG4 = RCX, // Fourth argument
    ARG5 = R8,  // Fifth argument
    ARG6 = R9,  // Sixth argument
};

struct x86_linux_generator {
    arena *memory;
    FILE *out_file;

    i32 stack_offset;  // Current stack pointer offset
    i32 label_counter; // For generating unique labels

    const char *current_procedure;
    bool in_procedure;
};

enum class x86_windows_register {
    RAX,
    RBX,
    RCX,
    RDX,
    RSI,
    RDI,
    RBP,
    RSP,
    R8,
    R9,
    R10,
    R11,
    R12,
    R13,
    R14,
    R15,

    // 32-bit versions
    EAX,
    EBX,
    ECX,
    EDX,
    ESI,
    EDI,
    EBP,
    ESP,

    // For procedure arguments (Microsoft x64 calling convention)
    ARG1 = RDI, // First argument (integer/pointer)
    ARG2 = RSI, // Second argument (integer/pointer)
    ARG3 = RDX, // Third argument (integer/pointer)
    ARG4 = RCX, // Fourth argument (integer/pointer)
    // Fifth+ arguments go on the stack

    // XMM registers for floating point (first 4 args)
    // XMM0, XMM1, XMM2, XMM3 (would need separate enum for these)
};

// x86_generator_windows
struct x86_windows_generator {
    arena *memory;
    FILE *out_file;

    i32 stack_offset;  // Current stack pointer offset
    i32 label_counter; // For generating unique labels
    i32 shadow_space;  // Windows requires 32 bytes of shadow space

    const char *current_procedure;
    bool in_procedure;
};

bool x86_linux_generator_init(x86_linux_generator &cg, arena &memory, FILE *out_file);
void x86_linux_generator_deinit(x86_linux_generator &codegen);
bool x86_linux_generate_asm(x86_linux_generator &codegen, ast_node *program);

bool x86_windows_generator_init(x86_windows_generator &cg, arena &memory, FILE *out_file);
void x86_windows_generator_deinit(x86_windows_generator &codegen);
bool x86_windows_generate_asm(x86_windows_generator &codegen, ast_node *program);
