#pragma once

#include "collections.hpp"
#include "defines.hpp"
#include "memory.hpp"
#include <map>

struct semantic_analyzer;
struct ast_node;

enum class ir_value_kind : u32 {
    REGISTER,
    IMMEDIATE,
    GLOBAL,
    PROCEDURE,
    LABEL,
    STACK_SLOT,
};

struct ir_value {
    ir_value_kind kind;
    ast_node *type;

    union {
        struct {
            u32 id;
            const char *debug_name;
        } reg;

        struct {
            union {
                i64 int_val;
                f64 float_val;
                bool bool_val;
                const char *string_val;
            };
        } immediate;

        struct {
            const char *name;
        } global;

        struct {
            const char *name;
        } procedure;

        struct {
            u32 id;
            const char *name;
        } label;

        struct {
            u32 offset;       // Stack offset
            u32 size;         // Size in bytes
            const char *name; // Variable name
        } stack_slot;
    };
};

enum class hir_opcode : u32 {
    // Arithmetic
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    NEG,

    // Comparison
    EQ,
    NE,
    LT,
    LE,
    GT,
    GE,

    // Memory
    LOAD,
    STORE,
    ALLOC,
    DEALLOC,

    // Control flow
    BR,
    BR_COND,
    CALL,
    RET,

    // Language-specific
    ARRAY_BOUNDS_CHECK,
    NULL_CHECK,
    OVERFLOW_CHECK,
    CAST,

    // High-level constructs
    SLICE_CREATE,
    STR_CONCAT,
    STRUCT_ACCESS,

    // Special
    SIZEOF,
    ALIGNOF,

    CONST,
    COPY,
    NOP,
};

struct hir_instruction {
    ir_value result;
    ir_value operands[3];
    u32 operand_count;
    hir_opcode opcode;

    ast_node *source_type;
    ast_node *source_node;
    bool can_overflow : 1;
    bool is_counds_checked : 1;
    const char *debug_info;

    u32 source_line, source_column;
};

struct hir_basic_block {
    u32 id;
    const char *label;

    pool<hir_instruction> instructions;

    hir_basic_block *successor1; // For BR or true branch of BR_COND
    hir_basic_block *successor2; // For false branch of BR_COND
    pool<hir_basic_block *> predecessors;

    bool visited;
    bool is_entry;
    bool is_exit;
};

struct hir_procedure {
    const char *name;
    ast_node *return_type;

    pool<ir_value> parameters;

    pool<hir_basic_block> basic_blocks;
    hir_basic_block *entry_block;

    u32 next_register_id;
    u32 next_block_id;

    u32 stack_size;
    pool<ir_value> stack_slots;

    bool is_builtin;
    bool has_varargs;
    ast_node *source_decl;
};

struct hir_pipeline {
    const char *name;

    pool<hir_procedure> procedures;
    pool<ir_value> globals;
    pool<ir_value> string_literals;

    arena *allocator;
    u32 next_string_id;
};

enum class lir_opcode : u32 {
    // Machine arithmetic (size-specific)
    ADD_I8,
    ADD_I16,
    ADD_I32,
    ADD_I64,
    SUB_I8,
    SUB_I16,
    SUB_I32,
    SUB_I64,
    MUL_I8,
    MUL_I16,
    MUL_I32,
    MUL_I64,
    // ... size-specific operations

    // Machine comparisons
    ICMP_EQ,
    ICMP_NE,
    ICMP_SLT,
    ICMP_SGT, // Signed
    ICMP_ULT,
    ICMP_UGT, // Unsigned
    FCMP_EQ,
    FCMP_LT,
    FCMP_GT, // Float

    // Explicit memory operations
    LOAD_I8,
    LOAD_I16,
    LOAD_I32,
    LOAD_I64,
    STORE_I8,
    STORE_I16,
    STORE_I32,
    STORE_I64,

    // Machine-level control flow
    BR,
    BR_COND,
    CALL,
    RET,

    // Calling convention operations
    PARAM_SETUP,  // Set up procedure parameters
    CALL_PREP,    // Prepare for procedure call
    CALL_CLEANUP, // Clean up after call

    // Register operations
    MOVE,   // Register-to-register copy
    SPILL,  // Register to stack
    RELOAD, // Stack to register

    // Memory management
    STACK_ALLOC, // Allocate stack space
    HEAP_ALLOC,  // Allocate heap memory

    // Platform-specific
    SYSCALL,    // System call
    INLINE_ASM, // Inline assembly
};

struct lir_instruction {
    lir_opcode opcode;
    ir_value result;
    ir_value operands[3];
    u32 operand_count;

    // Low-level metadata
    u32 instruction_size;     // Size in bytes for codegen
    bool is_commutative;      // Can swap operands
    bool clobbers_flags;      // Affects processor flags
    const char *asm_template; // For assembly generation
};

struct lir_basic_block {
    u32 id;
    const char *label;

    pool<lir_instruction> instructions;

    lir_basic_block *successor1;
    lir_basic_block *successor2;
    pool<lir_basic_block *> predecessors;

    bool visited;
    bool is_entry;
    bool is_exit;

    pool<ir_value> live_in;  // Live registers at block entry
    pool<ir_value> live_out; // Live registers at block exit
};

struct lir_procedure {
    const char *name;
    ast_node *return_type;

    pool<ir_value> parameters;

    pool<lir_basic_block> basic_blocks;
    lir_basic_block *entry_block;

    u32 next_virtual_register;
    u32 next_block_id;
    u32 total_stack_size;

    u32 param_stack_size;
    u32 local_stack_size;
    u32 spill_stack_size;

    bool uses_frame_pointer;
    bool needs_stack_alignment;

    hir_procedure *hir_source;
};

struct lir_pipeline {
    const char *name;

    pool<lir_procedure> procedures;
    pool<ir_value> globals;
    pool<ir_value> string_literals;

    const char *target_triple;
    u32 pointer_size;
    u32 stack_alignment;

    arena *allocator;

    hir_pipeline *hir_source;
};

struct hir_builder {
    hir_pipeline *pipeline;
    hir_procedure *current_procedure;
    hir_basic_block *current_block;

    u32 next_temp_id;
    arena *allocator;
};

struct lir_builder {
    lir_pipeline *pipeline;
    lir_procedure *current_procedure;
    lir_basic_block *current_block;

    u32 next_temp_id;
    arena *allocator;
};

struct ir_pipeline {
    arena *allocator;

    struct hir_pipeline *hir_pipeline;
    struct lir_pipeline *lir_pipeline;

    struct hir_builder *hir_builder;
    struct lir_builder *lir_builder;

    struct {
        std::map<hir_procedure *, lir_procedure *> procedure_mapping;
        std::map<hir_basic_block *, lir_basic_block *> block_mapping;
        std::map<ir_value, ir_value> value_mapping;
    } lowering_context;
};

bool ir_pipeline_init(ir_pipeline &pipeline, arena &allocator);
void ir_pipeline_deinit(ir_pipeline &pipeline);

bool hir_pipeline_init(hir_pipeline &pipeline, arena &allocator, const char *name);
void hir_pipeline_deinit(hir_pipeline &pipeline);

bool lir_pipeline_init(lir_pipeline &pipeline, arena &allocator, const char *name);
void lir_pipeline_deinit(lir_pipeline &pipeline);

bool hir_builder_init(hir_builder &builder, hir_pipeline &pipeline, arena &allocator);
bool hir_builder_set_procedure(hir_builder &builder, hir_procedure &procedure);
bool hir_builder_set_block(hir_builder &builder, hir_basic_block &block);

bool lir_builder_init(lir_builder &builder, lir_pipeline &pipeline, arena &allocator);
bool lir_builder_set_procedure(lir_builder &builder, lir_procedure &procedure);
bool lir_builder_set_block(lir_builder &builder, lir_basic_block &block);

hir_procedure *hir_create_procedure(hir_pipeline &pipeline, const char *name, ast_node *return_type);
hir_basic_block *hir_create_basic_block(hir_procedure &procedure, const char *label);

lir_procedure *lir_create_procedure(lir_pipeline &pipeline, const char *name, ast_node *return_type);
lir_basic_block *lir_create_basic_block(lir_procedure &procedure, const char *label);

// Shared between IR levels
ir_value ir_create_register(u32 id, ast_node *type, const char *debug_name = nullptr);
ir_value ir_create_immediate_int(i64 value, ast_node *type);
ir_value ir_create_immediate_float(f64 value, ast_node *type);
ir_value ir_create_immediate_bool(bool value, ast_node *type);
ir_value ir_create_immediate_string(const char *value, ast_node *type);
ir_value ir_create_global(const char *name, ast_node *type);
ir_value ir_create_procedure_ref(const char *name, ast_node *type);
ir_value ir_create_stack_slot(u32 offset, u32 size, const char *name, ast_node *type);

ir_value hir_emit_binary(hir_builder &builder, hir_opcode op, ir_value left, ir_value right, ast_node *result);
ir_value hir_emit_unary(hir_builder &builder, hir_opcode op, ir_value operand, ast_node *result_type);
ir_value hir_emit_load(hir_builder &builder, ir_value address, ast_node *result_type);
void hir_emit_store(hir_builder &builder, ir_value address, ir_value value);
ir_value hir_emit_call(hir_builder &builder, ir_value function, ir_value *args, u32 arg_count, ast_node *result_type);
void hir_emit_branch(hir_builder &builder, hir_basic_block *target);
void hir_emit_branch_cond(hir_builder &builder, ir_value condition, hir_basic_block *true_target,
                          hir_basic_block *false_target);
void hir_emit_return(hir_builder &builder, ir_value value);

ir_value lir_emit_binary(lir_builder &builder, lir_opcode op, ir_value left, ir_value right, ast_node *result_type);
ir_value lir_emit_unary(lir_builder &builder, lir_opcode op, ir_value operand, ast_node *result_type);
ir_value lir_emit_load(lir_builder &builder, ir_value address, ast_node *result_type);
void lir_emit_store(lir_builder &builder, ir_value address, ir_value value);
void lir_emit_branch(lir_builder &builder, lir_basic_block *target);
void lir_emit_branch_cond(lir_builder &builder, ir_value condition, lir_basic_block *true_target,
                          lir_basic_block *false_target);
void lir_emit_return(lir_builder &builder, ir_value value);

// IR lowering
lir_pipeline *lower_hir_to_lir(ir_pipeline &pipeline, hir_pipeline *hir);
lir_procedure *lower_hir_procedure_to_lir(ir_pipeline &pipeline, hir_procedure *hir_func);
lir_basic_block *lower_hir_block_to_lir(ir_pipeline &pipeline, hir_basic_block *hir_block);
ir_value lower_hir_instruction_to_lir(ir_pipeline &pipeline, hir_instruction *hir_instr);

const char *hir_opcode_to_string(hir_opcode op);
const char *lir_opcode_to_string(lir_opcode op);
bool ir_value_equals(const ir_value &a, const ir_value &b);
bool ir_instruction_is_terminator(hir_opcode op);
bool ir_instruction_is_terminator(lir_opcode op);

// Debug/printing
void hir_print_instruction(const hir_instruction &instr);
void hir_print_basic_block(const hir_basic_block &block);
void hir_print_function(const hir_procedure &procedure);
void hir_print(const hir_pipeline &pipeline);

void lir_print_instruction(const lir_instruction &instr);
void lir_print_basic_block(const lir_basic_block &block);
void lir_print_function(const lir_procedure &procedure);
void lir_print(const lir_pipeline &pipeline);
