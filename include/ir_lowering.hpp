#pragma once

#include "hir.hpp"
#include "ir.hpp"
#include "lir.hpp"

// =================================================================
// HIR TO LIR LOWERING
// =================================================================

struct hir_to_lir_lowering_context {
    ir_pipeline *pipeline;

    // Current context during lowering
    lir_procedure *current_lir_procedure;
    lir_basic_block *current_lir_block;
    lir_builder *lir_builder;

    // Target-specific information
    const char *target_triple;
    u32 pointer_size;
    u32 stack_alignment;

    // Type size cache
    hash_map<ast_node *, u32> type_sizes;
    hash_map<ast_node *, u32> type_alignments;

    // Error tracking
    bool has_errors;
    const char *error_message;
};

// =================================================================
// MAIN LOWERING INTERFACE
// =================================================================

bool hir_to_lir_lowering_init(hir_to_lir_lowering_context &context, ir_pipeline &pipeline);
void hir_to_lir_lowering_deinit(hir_to_lir_lowering_context &context);

// Top-level lowering functions
lir_module *lower_hir_to_lir(ir_pipeline &pipeline, hir_module *hir);
lir_procedure *lower_hir_procedure_to_lir(ir_pipeline &pipeline, hir_procedure *hir_proc);
lir_basic_block *lower_hir_block_to_lir(ir_pipeline &pipeline, hir_basic_block *hir_block);

// =================================================================
// INSTRUCTION LOWERING
// =================================================================

// Lower individual HIR instructions to LIR
bool lower_hir_instruction_to_lir(hir_to_lir_lowering_context &context, hir_instruction *hir_instr,
                                  lir_basic_block *target_block);

// Specific instruction lowering
void lower_hir_binary_op(hir_to_lir_lowering_context &context, hir_instruction *instr);
void lower_hir_unary_op(hir_to_lir_lowering_context &context, hir_instruction *instr);
void lower_hir_load(hir_to_lir_lowering_context &context, hir_instruction *instr);
void lower_hir_store(hir_to_lir_lowering_context &context, hir_instruction *instr);
void lower_hir_call(hir_to_lir_lowering_context &context, hir_instruction *instr);
void lower_hir_branch(hir_to_lir_lowering_context &context, hir_instruction *instr);
void lower_hir_branch_cond(hir_to_lir_lowering_context &context, hir_instruction *instr);
void lower_hir_return(hir_to_lir_lowering_context &context, hir_instruction *instr);

// High-level construct lowering
void lower_hir_array_bounds_check(hir_to_lir_lowering_context &context, hir_instruction *instr);
void lower_hir_null_check(hir_to_lir_lowering_context &context, hir_instruction *instr);
void lower_hir_overflow_check(hir_to_lir_lowering_context &context, hir_instruction *instr);
void lower_hir_cast(hir_to_lir_lowering_context &context, hir_instruction *instr);
void lower_hir_sizeof(hir_to_lir_lowering_context &context, hir_instruction *instr);

// =================================================================
// OPCODE MAPPING
// =================================================================

// Convert HIR opcodes to LIR opcodes based on type
lir_opcode hir_binary_op_to_lir_opcode(hir_opcode hir_op, ast_node *type);
lir_opcode hir_comparison_to_lir_opcode(hir_opcode hir_op, ast_node *type);
lir_opcode hir_load_to_lir_opcode(ast_node *type);
lir_opcode hir_store_to_lir_opcode(ast_node *type);

// Type-specific operation selection
lir_opcode select_arithmetic_opcode(hir_opcode hir_op, ast_node *type);
lir_opcode select_comparison_opcode(hir_opcode hir_op, ast_node *type, bool is_signed);

// =================================================================
// VALUE LOWERING
// =================================================================

// Lower HIR values to LIR values
ir_value lower_hir_value_to_lir(hir_to_lir_lowering_context &context, ir_value hir_value);

// Value type conversions
ir_value convert_hir_register_to_lir(hir_to_lir_lowering_context &context, ir_value hir_reg);
ir_value convert_hir_immediate_to_lir(hir_to_lir_lowering_context &context, ir_value hir_imm);
ir_value convert_hir_global_to_lir(hir_to_lir_lowering_context &context, ir_value hir_global);

// =================================================================
// TYPE ANALYSIS FOR LOWERING
// =================================================================

// Type size and alignment calculation
u32 get_type_size_bytes(hir_to_lir_lowering_context &context, ast_node *type);
u32 get_type_alignment_bytes(hir_to_lir_lowering_context &context, ast_node *type);
bool is_signed_integer_type(ast_node *type);
bool is_unsigned_integer_type(ast_node *type);
bool is_floating_point_type(ast_node *type);
bool is_pointer_type(ast_node *type);

// Type classification for lowering decisions
enum class lowering_type_class {
    INTEGER_8,
    INTEGER_16,
    INTEGER_32,
    INTEGER_64,
    FLOAT_32,
    FLOAT_64,
    POINTER,
    STRUCT,
    ARRAY,
    UNKNOWN
};

lowering_type_class classify_type_for_lowering(hir_to_lir_lowering_context &context, ast_node *type);

// =================================================================
// CALLING CONVENTION LOWERING
// =================================================================

struct call_lowering_info {
    pool<ir_value> argument_values;  // LIR values for arguments
    pool<lir_opcode> argument_setup; // Setup instructions needed
    ir_value return_value;           // Where return value goes
    u32 stack_adjustment;            // Stack space needed for call
    bool needs_cleanup;              // Whether cleanup is required
};

// Lower HIR function calls to LIR with proper calling conventions
call_lowering_info lower_call_with_calling_convention(hir_to_lir_lowering_context &context,
                                                      hir_instruction *call_instr);

// Calling convention helpers
void setup_call_arguments(hir_to_lir_lowering_context &context, ir_value *args, u32 arg_count,
                          call_lowering_info &info);
void generate_call_cleanup(hir_to_lir_lowering_context &context, const call_lowering_info &info);

// =================================================================
// CONTROL FLOW LOWERING
// =================================================================

// Lower HIR control flow to LIR
void lower_hir_control_flow(hir_to_lir_lowering_context &context, hir_basic_block *hir_block);

// Conditional branch lowering with complex conditions
struct branch_lowering_info {
    lir_basic_block *true_target;
    lir_basic_block *false_target;
    pool<lir_instruction> condition_setup; // Instructions to evaluate condition
    ir_value final_condition;              // Final boolean value for branch
};

branch_lowering_info lower_complex_branch_condition(hir_to_lir_lowering_context &context, ir_value condition,
                                                    lir_basic_block *true_target, lir_basic_block *false_target);

// =================================================================
// MEMORY OPERATION LOWERING
// =================================================================

// Lower HIR memory operations to target-specific LIR
void lower_hir_memory_operation(hir_to_lir_lowering_context &context, hir_instruction *instr);

// Address calculation lowering
ir_value lower_address_calculation(hir_to_lir_lowering_context &context, ir_value base_address, ir_value offset);

// Struct field access lowering
struct field_access_info {
    u32 field_offset;
    u32 field_size;
    ast_node *field_type;
};

field_access_info get_field_access_info(hir_to_lir_lowering_context &context, ast_node *struct_type,
                                        const char *field_name);

ir_value lower_struct_field_access(hir_to_lir_lowering_context &context, ir_value struct_ptr,
                                   const field_access_info &field_info);

// Array access lowering
ir_value lower_array_access(hir_to_lir_lowering_context &context, ir_value array_ptr, ir_value index,
                            ast_node *element_type);

// =================================================================
// OPTIMIZATION HINTS DURING LOWERING
// =================================================================

// Generate optimization-friendly LIR
struct lowering_optimization_hints {
    bool prefer_registers_over_stack;
    bool enable_instruction_combining;
    bool optimize_for_size;
    bool optimize_for_speed;
};

void set_lowering_optimization_hints(hir_to_lir_lowering_context &context, const lowering_optimization_hints &hints);

// Common optimization patterns during lowering
void try_combine_load_store(hir_to_lir_lowering_context &context, hir_instruction *instr);
void try_strength_reduction(hir_to_lir_lowering_context &context, hir_instruction *instr);
void try_constant_folding(hir_to_lir_lowering_context &context, hir_instruction *instr);

// =================================================================
// ERROR HANDLING DURING LOWERING
// =================================================================

// Error reporting for lowering phase
void lowering_error(hir_to_lir_lowering_context &context, const char *message, hir_instruction *instr);
void lowering_warning(hir_to_lir_lowering_context &context, const char *message, hir_instruction *instr);

bool has_lowering_errors(const hir_to_lir_lowering_context &context);
void clear_lowering_errors(hir_to_lir_lowering_context &context);

// =================================================================
// TARGET-SPECIFIC LOWERING STRATEGIES
// =================================================================

// Different lowering strategies for different targets
enum class lowering_strategy {
    GENERIC, // Generic lowering, works on all targets
    X86_64,  // x86-64 specific optimizations
    AARCH64, // ARM64 specific optimizations
    RISCV,   // RISC-V specific optimizations
};

lowering_strategy select_lowering_strategy(const char *target_triple);
void apply_target_specific_lowering(hir_to_lir_lowering_context &context, lowering_strategy strategy);

// Target-specific instruction patterns
struct target_instruction_pattern {
    const char *pattern_name;
    bool (*matches)(hir_instruction *instr);
    void (*apply)(hir_to_lir_lowering_context &context, hir_instruction *instr);
    u32 estimated_cycles_saved;
};

void register_target_patterns(hir_to_lir_lowering_context &context, lowering_strategy strategy);
void apply_target_patterns(hir_to_lir_lowering_context &context, hir_instruction *instr);

// =================================================================
// DEBUGGING AND VALIDATION
// =================================================================

// Debug lowering process
void debug_print_lowering_step(hir_to_lir_lowering_context &context, hir_instruction *hir_instr,
                               lir_instruction *lir_instr);

// Validate lowering correctness
bool validate_lowering(hir_to_lir_lowering_context &context, hir_procedure *hir_proc, lir_procedure *lir_proc);

// Lowering statistics
struct lowering_statistics {
    u32 hir_instructions_processed;
    u32 lir_instructions_generated;
    u32 expansion_ratio; // LIR/HIR * 100
    u32 target_patterns_applied;
    u32 optimizations_applied;
    f64 lowering_time_ms;
};

lowering_statistics get_lowering_statistics(const hir_to_lir_lowering_context &context);
void print_lowering_statistics(const lowering_statistics &stats);

// =================================================================
// EXAMPLE LOWERING PATTERNS
// =================================================================

/*
// Example: HIR ADD -> LIR size-specific ADD

HIR:
%result = add i32 %a, %b

LIR:
%result = add_i32 %a, %b

// Example: HIR bounds check -> LIR compare + branch

HIR:
array_bounds_check %array, %index

LIR:
%array_len = load_i32 %array, 0     ; Load length field
%cmp = icmp_uge %index, %array_len   ; Compare index >= length
br_cond %cmp, %panic_block, %ok_block

%panic_block:
call @panic("Array index out of bounds")

%ok_block:
; Continue with array access

// Example: HIR call -> LIR calling convention

HIR:
%result = call @foo(%arg1, %arg2, %arg3)

LIR (x86-64):
; Arguments in registers: RDI, RSI, RDX
move %arg1, %rdi
move %arg2, %rsi
move %arg3, %rdx
call @foo
move %rax, %result    ; Return value in RAX

// Example: HIR struct access -> LIR offset calculation

HIR:
%field = struct_access %struct_ptr, "field_name"

LIR:
%field_addr = add_i64 %struct_ptr, 16  ; field offset = 16
%field = load_i32 %field_addr          ; load field value
*/
