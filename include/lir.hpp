#pragma once

#include "collections.hpp"
#include "ir.hpp"

struct ast_node;
struct hir_module;
struct hir_procedure;

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
    DIV_I8,
    DIV_I16,
    DIV_I32,
    DIV_I64,
    MOD_I8,
    MOD_I16,
    MOD_I32,
    MOD_I64,

    // Floating point arithmetic
    ADD_F32,
    ADD_F64,
    SUB_F32,
    SUB_F64,
    MUL_F32,
    MUL_F64,
    DIV_F32,
    DIV_F64,

    // Machine comparisons
    ICMP_EQ,
    ICMP_NE,
    ICMP_SLT,
    ICMP_SLE,
    ICMP_SGT,
    ICMP_SGE, // Signed
    ICMP_ULT,
    ICMP_ULE,
    ICMP_UGT,
    ICMP_UGE, // Unsigned
    FCMP_EQ,
    FCMP_NE,
    FCMP_LT,
    FCMP_LE,
    FCMP_GT,
    FCMP_GE, // Float

    // Explicit memory operations
    LOAD_I8,
    LOAD_I16,
    LOAD_I32,
    LOAD_I64,
    LOAD_F32,
    LOAD_F64,
    STORE_I8,
    STORE_I16,
    STORE_I32,
    STORE_I64,
    STORE_F32,
    STORE_F64,

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

    // Type conversions
    SEXT,   // Sign extend
    ZEXT,   // Zero extend
    TRUNC,  // Truncate
    SITOFP, // Signed int to float
    UITOFP, // Unsigned int to float
    FPTOSI, // Float to signed int
    FPTOUI, // Float to unsigned int

    // Platform-specific
    SYSCALL,    // System call
    INLINE_ASM, // Inline assembly

    // Misc
    NOP,  // No operation
    COPY, // Simple copy
};

struct lir_instruction {
    lir_opcode opcode;
    ir_value result;
    ir_value operands[3];
    u32 operand_count;

    // Low-level metadata
    u32 instruction_size;     // Size in bytes for codegen
    bool is_commutative : 1;  // Can swap operands
    bool clobbers_flags : 1;  // Affects processor flags
    const char *asm_template; // For assembly generation

    // Source tracking
    u32 source_line, source_column;
};

struct lir_basic_block {
    u32 id;
    const char *label;

    pool<lir_instruction> instructions;

    lir_basic_block *successor1;
    lir_basic_block *successor2;
    pool<lir_basic_block *> predecessors;

    bool visited : 1;
    bool is_entry : 1;
    bool is_exit : 1;

    // Register allocation data
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

    // Calling convention info
    u32 param_stack_size;
    u32 local_stack_size;
    u32 spill_stack_size;

    // Target-specific metadata
    bool uses_frame_pointer : 1;
    bool needs_stack_alignment : 1;

    // Original HIR procedure (for debugging)
    hir_procedure *hir_source;
};

struct lir_module {
    const char *name;

    pool<lir_procedure> procedures;
    pool<ir_value> globals;
    pool<ir_value> string_literals;

    // Target-specific information
    const char *target_triple; // e.g., "x86_64-pc-linux-gnu"
    u32 pointer_size;          // 4 or 8 bytes
    u32 stack_alignment;       // Required stack alignment

    arena *allocator;
    u32 next_string_id;

    // Reference to original HIR (for debugging)
    hir_module *hir_source;
};

struct lir_builder {
    lir_module *module;
    lir_procedure *current_procedure;
    lir_basic_block *current_block;

    u32 next_temp_id;
    arena *allocator;
};

// =================================================================
// CORE LIR FUNCTIONS
// =================================================================

// Module management
bool lir_module_init(lir_module &module, arena &allocator, const char *name);
void lir_module_deinit(lir_module &module);

// Builder management
bool lir_builder_init(lir_builder &builder, lir_module &module, arena &allocator);
bool lir_builder_set_procedure(lir_builder &builder, lir_procedure &procedure);
bool lir_builder_set_block(lir_builder &builder, lir_basic_block &block);

// Procedure and block creation
lir_procedure *lir_create_procedure(lir_module &module, const char *name, ast_node *return_type);
lir_basic_block *lir_create_basic_block(lir_procedure &procedure, const char *label);

// Instruction emission
ir_value lir_emit_binary(lir_builder &builder, lir_opcode op, ir_value left, ir_value right, ast_node *result_type);
ir_value lir_emit_unary(lir_builder &builder, lir_opcode op, ir_value operand, ast_node *result_type);
ir_value lir_emit_load(lir_builder &builder, ir_value address, ast_node *result_type);
void lir_emit_store(lir_builder &builder, ir_value address, ir_value value);
ir_value lir_emit_call(lir_builder &builder, ir_value procedure, ir_value *args, u32 arg_count, ast_node *result_type);
void lir_emit_branch(lir_builder &builder, lir_basic_block *target);
void lir_emit_branch_cond(lir_builder &builder, ir_value condition, lir_basic_block *true_target,
                          lir_basic_block *false_target);
void lir_emit_return(lir_builder &builder, ir_value value);
void lir_emit_return_void(lir_builder &builder);

// Special LIR operations
ir_value lir_emit_convert(lir_builder &builder, ir_value value, ast_node *target_type);
ir_value lir_emit_stack_alloc(lir_builder &builder, u32 size, u32 alignment, const char *name);
void lir_emit_call_setup(lir_builder &builder, ir_value *args, u32 arg_count);
void lir_emit_call_cleanup(lir_builder &builder, u32 stack_adjustment);

// Control flow helpers
lir_basic_block *lir_create_and_switch_block(lir_builder &builder, const char *label);
bool lir_block_is_terminated(lir_basic_block *block);

// Utility functions
const char *lir_opcode_to_string(lir_opcode op);
bool lir_instruction_is_terminator(lir_opcode op);
bool lir_instruction_has_side_effects(lir_opcode op);
bool lir_instruction_is_commutative(lir_opcode op);

// Instruction analysis
bool lir_instruction_reads_memory(const lir_instruction &instr);
bool lir_instruction_writes_memory(const lir_instruction &instr);
bool lir_instruction_is_move(const lir_instruction &instr);

// Debug/printing
void lir_print_instruction(const lir_instruction &instr);
void lir_print_basic_block(const lir_basic_block &block);
void lir_print_procedure(const lir_procedure &procedure);
void lir_print_module(const lir_module &module);

// =================================================================
// LIR OPTIMIZATION PASS FRAMEWORK
// =================================================================

struct lir_optimization_pass {
    const char *name;
    bool (*run_on_module)(lir_module *module, struct lir_pass_manager *manager);
    bool (*run_on_procedure)(lir_procedure *procedure, struct lir_pass_manager *manager);
    bool (*run_on_basic_block)(lir_basic_block *block, struct lir_pass_manager *manager);

    // Pass metadata
    bool modifies_cfg : 1;          // Changes control flow graph
    bool modifies_instructions : 1; // Changes instructions
    bool requires_liveness : 1;     // Needs liveness analysis
    bool requires_dominance : 1;    // Needs dominance analysis

    // Dependencies
    const char **required_passes;
    u32 required_pass_count;

    // Invalidated analyses
    const char **invalidates;
    u32 invalidates_count;
};

struct lir_pass_manager {
    arena *allocator;

    pool<lir_optimization_pass *> passes;
    hash_map<const char *, lir_optimization_pass *> pass_lookup;

    hash_map<const char *, void *> analysis_cache;
    hash_map<const char *, bool> analysis_valid;

    u32 total_passes_run;
    u32 total_procedures_optimized;
    bool debug_mode : 1;
};

bool lir_pass_manager_init(lir_pass_manager &manager, arena &allocator);
void lir_pass_manager_deinit(lir_pass_manager &manager);
void lir_pass_manager_register_pass(lir_pass_manager &manager, lir_optimization_pass *pass);
bool lir_optimize_module(lir_module *module, lir_pass_manager &manager);

// =================================================================
// COMMON LIR ANALYSES
// =================================================================

struct lir_liveness_analysis {
    hash_map<lir_basic_block *, pool<ir_value>> live_in;
    hash_map<lir_basic_block *, pool<ir_value>> live_out;
    hash_map<lir_instruction *, pool<ir_value>> live_after;
};

struct lir_register_interference {
    hash_map<ir_value, pool<ir_value>> conflicts; // Which registers conflict
    hash_map<ir_value, u32> spill_cost;           // Cost to spill each register
};

// Analysis computation
lir_liveness_analysis *compute_liveness_analysis(lir_procedure *procedure, lir_pass_manager *manager);
lir_register_interference *compute_register_interference(lir_procedure *procedure, lir_pass_manager *manager);

// =================================================================
// COMMON LIR OPTIMIZATION PASSES
// =================================================================

// Register allocation
struct lir_register_allocation_pass {
    lir_optimization_pass base;

    u32 available_registers;
    pool<ir_value> spilled_values;
    hash_map<ir_value, u32> register_assignment;

    u32 registers_allocated;
    u32 values_spilled;
};

bool run_register_allocation(lir_module *module, lir_pass_manager *manager);

// Peephole optimization
struct lir_peephole_pass {
    lir_optimization_pass base;

    u32 instructions_eliminated;
    u32 instructions_combined;
};

bool run_peephole_optimization(lir_module *module, lir_pass_manager *manager);

// Instruction scheduling
struct lir_instruction_scheduling_pass {
    lir_optimization_pass base;

    u32 instructions_reordered;
    u32 cycles_saved;
};

bool run_instruction_scheduling(lir_module *module, lir_pass_manager *manager);

// Register coalescing
struct lir_register_coalescing_pass {
    lir_optimization_pass base;

    u32 moves_eliminated;
    u32 registers_coalesced;
};

bool run_register_coalescing(lir_module *module, lir_pass_manager *manager);

// =================================================================
// TARGET-SPECIFIC HELPERS
// =================================================================

// Get target-specific information
u32 get_target_register_count(const char *target_triple);
u32 get_target_stack_alignment(const char *target_triple);
const char **get_target_register_names(const char *target_triple);

// Calling convention helpers
struct calling_convention_info {
    pool<u32> argument_registers; // Registers for arguments
    pool<u32> return_registers;   // Registers for return values
    pool<u32> caller_saved;       // Caller must save these
    pool<u32> callee_saved;       // Callee must save these
    u32 stack_alignment;
    bool arguments_right_to_left;
};

calling_convention_info *get_calling_convention(const char *target_triple);

// =================================================================
// REGISTER ALLOCATION HELPERS
// =================================================================

struct physical_register {
    u32 id;
    const char *name;
    bool is_available;
    bool is_caller_saved;
    bool is_callee_saved;
};

struct register_allocator {
    pool<physical_register> available_registers;
    hash_map<ir_value, u32> virtual_to_physical;
    hash_map<u32, ir_value> physical_to_virtual;

    lir_register_interference *interference;
    lir_liveness_analysis *liveness;
};

bool allocate_registers_for_procedure(lir_procedure *procedure, register_allocator &allocator);
u32 select_register_for_value(ir_value value, register_allocator &allocator);
bool need_to_spill_register(u32 physical_reg, ir_value for_value, register_allocator &allocator);
