#pragma once

#include "collections.hpp"
#include "ir.hpp"

struct ast_node;

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

struct hir_module {
    const char *name;

    pool<hir_procedure> procedures;
    pool<ir_value> globals;
    pool<ir_value> string_literals;

    arena *allocator;
    u32 next_string_id;
};

bool hir_module_init(hir_module &module, arena &allocator, const char *name);
void hir_module_deinit(hir_module &module);

bool hir_builder_init(hir_builder &builder, hir_module &module, arena &allocator);
bool hir_builder_set_procedure(hir_builder &builder, hir_procedure &procedure);
bool hir_builder_set_block(hir_builder &builder, hir_basic_block &block);

hir_procedure *hir_create_procedure(hir_module &module, const char *name, ast_node *return_type);
hir_basic_block *hir_create_basic_block(hir_procedure &procedure, const char *label);

ir_value hir_emit_binary(hir_builder &builder, hir_opcode op, ir_value left, ir_value right, ast_node *result);
ir_value hir_emit_unary(hir_builder &builder, hir_opcode op, ir_value operand, ast_node *result_type);
ir_value hir_emit_load(hir_builder &builder, ir_value address, ast_node *result_type);
void hir_emit_store(hir_builder &builder, ir_value address, ir_value value);
ir_value hir_emit_call(hir_builder &builder, ir_value procedure, ir_value *args, u32 arg_count, ast_node *result_type);
void hir_emit_branch(hir_builder &builder, hir_basic_block *target);
void hir_emit_branch_cond(hir_builder &builder, ir_value condition, hir_basic_block *true_target,
                          hir_basic_block *false_target);
void hir_emit_return(hir_builder &builder, ir_value value);

const char *hir_opcode_to_string(hir_opcode op);
void hir_print_instruction(const hir_instruction &instr);
void hir_print_basic_block(const hir_basic_block &block);
void hir_print_procedure(const hir_procedure &procedure);
void hir_print_module(const hir_module &module);
bool ir_instruction_is_terminator(hir_opcode op);

struct hir_optimization_pass {
    const char *name;
    bool (*run_on_module)(hir_module *module, struct hir_pass_manager *manager);
    bool (*run_on_procedure)(hir_procedure *procedure, struct hir_pass_manager *manager);
    bool (*run_on_basic_block)(hir_basic_block *block, struct hir_pass_manager *manager);

    // Pass metadata
    bool modifies_cfg;          // Changes control flow graph
    bool modifies_instructions; // Changes instructions
    bool requires_dominator_tree;
    bool requires_ssa_form;

    // Dependencies - passes that must run before this one
    const char **required_passes;
    u32 required_pass_count;

    // Invalidated analyses - what this pass makes obsolete
    const char **invalidates;
    u32 invalid_count;
};

struct hir_pass_manager {
    arena *allocator;

    pool<hir_optimization_pass> passes;
    hash_map<const char *, hir_optimization_pass> pass_lookup;

    hash_map<const char *, void *> analysis_cache;
    hash_map<const char *, bool> analysis_valid;

    u32 total_passes_run;
    u32 total_procedures_optimized;
    bool debug_mode;
};

bool hir_pass_manager_init(hir_pass_manager &manager);

struct hir_analysis {
    const char *name;
    void *(*compute)(hir_procedure *procedure, hir_pass_manager *manager);
    void (*invalidate)(void *analysis_data);
    bool is_procedure_pass; // vs module pass
};

// Common analyses
struct dominator_tree {
    hash_map<hir_basic_block *, hir_basic_block *> immediate_dominators;
    hash_map<hir_basic_block *, hir_basic_block *> dominated_blocks;
    hir_basic_block *root;
};

struct use_def_chains {
    hash_map<ir_value, pool<hir_instruction *>> uses;
    hash_map<ir_value, hir_instruction *> definitions;
};

struct live_variable_analysis {
    hash_map<hir_basic_block *, pool<ir_value>> live_in;  // Live at block entry
    hash_map<hir_basic_block *, pool<ir_value>> live_out; // Live at block exit
};

struct hir_dead_code_elimination_pass {
    hir_optimization_pass base;

    pool<hir_instruction *> worklist;
    hash_set<hir_instruction *> marked_live;

    u32 instructions_removed;
    u32 blocks_removed;
};

bool run_dead_code_elimination(hir_module *module, hir_pass_manager *manager);
bool is_instruction_dead(hir_instruction *instr, use_def_chains *ud_chains);
void mark_instruction_live(hir_instruction *instr, hir_dead_code_elimination_pass *pass);
void sweep_dead_instructions(hir_procedure *proc, hir_dead_code_elimination_pass *pass);

struct hir_constant_propagation_pass {
    hir_optimization_pass base;

    hash_map<ir_value, compile_time_value> known_constants;
    pool<hir_instruction *> worklist;

    u32 constants_folded;
    u32 instructions_simplified;
};

bool run_constant_propagation(hir_module *module, hir_pass_manager *manager);
bool try_fold_constant_instruction(hir_instruction *instr, hir_constant_propagation_pass *pass);
bool is_compile_time_constant(ir_value value, hir_constant_propagation_pass *pass);
compile_time_value evaluate_constant_binary_op(hir_opcode, compile_time_value left, compile_time_value right);

struct hir_procedure_inlining_pass {
    hir_optimization_pass base;

    hash_map<hir_procedure *, u32> inline_cost;
    hash_map<hir_procedure *, bool> inline_decisions;
    u32 max_inline_cost;
    u32 max_inline_depth;

    u32 procedures_inlined;
    u32 calls_inlined;
    u32 code_size_increase;
};

bool run_procedure_inlining(hir_module *module, hir_pass_manager *manager);
u32 calculate_inline_cost(hir_procedure *proc, hir_procedure_inlining_pass *pass);
bool should_inline_call(hir_instruction *call_instr, hir_procedure_inlining_pass *pass);
bool inline_procedure_call(hir_instruction *call_instr, hir_procedure *target, hir_procedure_inlining_pass *pass);

struct expression_key {
    hir_opcode opcode;
    ir_value operands[3];
    u32 operand_count;

    bool operator==(const expression_key &other) const;
    u32 hash() const;
};

// Common subexpression elimination
struct hir_cse_pass {
    hir_optimization_pass base;

    hash_map<expression_key, ir_value> available_expressions;
    pool<hir_instruction *> worklist;

    u32 expressions_eliminated;
};

bool run_common_subexpression_elimination(hir_module *module, hir_pass_manager *manager);
expression_key create_expression_key(hir_instruction *instr);
bool expressions_equivalent(hir_instruction *a, hir_instruction *b);
