#pragma once

#include "ast.hpp"
#include "memory.hpp"
#include <cstdio>

enum class reg {
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

struct x86_generator {
    arena_allocator *memory;
    FILE *out_file;

    i32 stack_offset;  // Current stack pointer offset
    i32 label_counter; // For generating unique labels

    const char *current_procedure;
    bool in_procedure;
};

bool x86_gen_init(x86_generator &cg, arena_allocator &memory, FILE *out_file);
void x86_gen_deinit(x86_generator &codegen);

bool generate_x86(x86_generator &codegen, ast_node *program);

void emit_comment(x86_generator &cg, const char *comment, ...);
void emit_label(x86_generator &cg, const char *label);
void emit_directive(x86_generator &cg, const char *directive);
void emit_instruction(x86_generator &cg, const char *instr);
void emit_instruction_reg(x86_generator &cg, const char *instr, reg r);
void emit_instruction_reg_reg(x86_generator &cg, const char *instr, reg dst, reg src);
void emit_instruction_reg_imm(x86_generator &cg, const char *instr, reg r, i32 imm);
void emit_instruction_reg_mem(x86_generator &cg, const char *instr, reg r, reg base, i32 offset);
