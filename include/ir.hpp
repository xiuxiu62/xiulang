#pragma once

#include "collections.hpp"
#include "defines.hpp"
#include "memory.hpp"

struct ast_node;
struct hir_builder;
struct hir_module;
struct hir_procedure;
struct hir_basic_block;
struct lir_builder;
struct lir_module;
struct lir_procedure;
struct lir_basic_block;

// =================================================================
// SHARED IR VALUE STRUCTURE
// =================================================================

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

    // Comparison operator for use in hash maps
    bool operator==(const ir_value &other) const;
    u32 hash() const;
};

// =================================================================
// IR PIPELINE COORDINATION
// =================================================================

struct ir_pipeline {
    arena *allocator;

    hir_module *hir;
    lir_module *lir;

    struct hir_builder *hir_builder;
    struct lir_builder *lir_builder;

    // Lowering context for HIR -> LIR translation
    struct {
        hash_map<hir_procedure *, lir_procedure *> procedure_mapping;
        hash_map<hir_basic_block *, lir_basic_block *> block_mapping;
        hash_map<ir_value, ir_value> value_mapping; // HIR value -> LIR value
    } lowering_context;

    // Compilation context
    const char *source_file_path;
    const char *target_triple;
    bool debug_info_enabled;
};

// =================================================================
// PIPELINE MANAGEMENT
// =================================================================

bool ir_pipeline_init(ir_pipeline &pipeline, arena &allocator);
void ir_pipeline_deinit(ir_pipeline &pipeline);

// Set target-specific information
void ir_pipeline_set_target(ir_pipeline &pipeline, const char *target_triple);
void ir_pipeline_enable_debug_info(ir_pipeline &pipeline, bool enabled);

// =================================================================
// SHARED IR VALUE FUNCTIONS
// =================================================================

// Value creation (shared between HIR and LIR)
ir_value ir_create_register(u32 id, ast_node *type, const char *debug_name = nullptr);
ir_value ir_create_immediate_int(i64 value, ast_node *type);
ir_value ir_create_immediate_float(f64 value, ast_node *type);
ir_value ir_create_immediate_bool(bool value, ast_node *type);
ir_value ir_create_immediate_string(const char *value, ast_node *type);
ir_value ir_create_global(const char *name, ast_node *type);
ir_value ir_create_procedure_ref(const char *name, ast_node *type);
ir_value ir_create_stack_slot(u32 offset, u32 size, const char *name, ast_node *type);

// Value utilities
bool ir_value_equals(const ir_value &a, const ir_value &b);
bool ir_value_is_constant(const ir_value &value);
bool ir_value_is_register(const ir_value &value);
bool ir_value_is_memory(const ir_value &value);

// Type queries
bool ir_value_type_is_integer(const ir_value &value);
bool ir_value_type_is_float(const ir_value &value);
bool ir_value_type_is_pointer(const ir_value &value);
u32 ir_value_get_size_bytes(const ir_value &value);

// Debug/printing
void ir_print_value(const ir_value &value);
const char *ir_value_kind_to_string(ir_value_kind kind);

// =================================================================
// COMPILE-TIME VALUE REPRESENTATION
// =================================================================

enum class compile_time_value_kind : u32 {
    INTEGER,
    FLOAT,
    BOOLEAN,
    STRING,
    NULL_PTR,
    UNDEFINED,
};

struct compile_time_value {
    compile_time_value_kind kind;
    ast_node *type;

    union {
        i64 int_value;
        f64 float_value;
        bool bool_value;
        const char *string_value;
    };

    bool is_valid() const {
        return kind != compile_time_value_kind::UNDEFINED;
    }
};

// Compile-time value operations
compile_time_value create_compile_time_int(i64 value, ast_node *type);
compile_time_value create_compile_time_float(f64 value, ast_node *type);
compile_time_value create_compile_time_bool(bool value, ast_node *type);
compile_time_value create_compile_time_string(const char *value, ast_node *type);
compile_time_value create_compile_time_null(ast_node *type);

bool compile_time_values_equal(const compile_time_value &a, const compile_time_value &b);
ir_value compile_time_value_to_ir_value(const compile_time_value &value);

// =================================================================
// FORWARD DECLARATIONS FOR HIR/LIR COORDINATION
// =================================================================

// These are implemented in their respective modules

// HIR/LIR lowering (implemented in hir_to_lir.cpp)
lir_module *lower_hir_to_lir(ir_pipeline &pipeline, hir_module *hir);
lir_procedure *lower_hir_procedure_to_lir(ir_pipeline &pipeline, hir_procedure *hir_proc);
lir_basic_block *lower_hir_block_to_lir(ir_pipeline &pipeline, hir_basic_block *hir_block);
ir_value lower_hir_value_to_lir(ir_pipeline &pipeline, ir_value hir_value);

// Register lowering context mappings
void register_procedure_mapping(ir_pipeline &pipeline, hir_procedure *hir_proc, lir_procedure *lir_proc);
void register_block_mapping(ir_pipeline &pipeline, hir_basic_block *hir_block, lir_basic_block *lir_block);
void register_value_mapping(ir_pipeline &pipeline, ir_value hir_value, ir_value lir_value);

// Lookup lowering mappings
lir_procedure *lookup_procedure_mapping(ir_pipeline &pipeline, hir_procedure *hir_proc);
lir_basic_block *lookup_block_mapping(ir_pipeline &pipeline, hir_basic_block *hir_block);
ir_value lookup_value_mapping(ir_pipeline &pipeline, ir_value hir_value);

// =================================================================
// UTILITY MACROS
// =================================================================

// For creating temporary register names
#define IR_TEMP_NAME(prefix, id) arena_printf(*pipeline.allocator, "%s%u", prefix, id)

// For type-safe value creation
#define IR_REG(id, type) ir_create_register(id, type, nullptr)
#define IR_IMM_INT(val, type) ir_create_immediate_int(val, type)
#define IR_IMM_FLOAT(val, type) ir_create_immediate_float(val, type)
#define IR_IMM_BOOL(val, type) ir_create_immediate_bool(val, type)

// =================================================================
// VALIDATION AND DEBUGGING
// =================================================================

// IR validation
bool validate_ir_value(const ir_value &value);
bool validate_ir_pipeline(const ir_pipeline &pipeline);

// Debug statistics
struct ir_statistics {
    u32 total_hir_instructions;
    u32 total_lir_instructions;
    u32 total_hir_blocks;
    u32 total_lir_blocks;
    u32 total_hir_procedures;
    u32 total_lir_procedures;
    u32 expansion_ratio; // LIR instructions / HIR instructions * 100
};

ir_statistics compute_ir_statistics(const ir_pipeline &pipeline);
void print_ir_statistics(const ir_statistics &stats);
