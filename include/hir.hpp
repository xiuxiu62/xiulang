#pragma once

#include "ir.hpp"

struct hir_optimization_pass {
    const char *name;
    bool (*run_on_pipeline)(hir_pipeline *pipeline, struct hir_pass_manager *manager);
};

struct hir_pass_manager {};
