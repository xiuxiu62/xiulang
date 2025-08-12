#pragma once

#include "defines.hpp"
// #include "platform.hpp"
#include "stdio.h"

#define info(fmt, ...) when_debug(fprintf(stdout, "[INFO] " fmt "\n", ##__VA_ARGS__))

#define warn(fmt, ...) when_debug(fprintf(stdout, "[WARN] " fmt "\n", ##__VA_ARGS__))

#define error(fmt, ...) when_debug(fprintf(stderr, "[ERROR] " fmt "\n", ##__VA_ARGS__))

#define unreachable()                                                                                                  \
    error("Reached an unreachable branch in %s (%s:%d)", __func__, __FILE__, __LINE__);                                \
    // platform_halt();                                                                                                   \
    throw "unreachable"

#define unimplemented()                                                                                                \
    error("Attempted to call an unimplmented function %s (%s:%d)", __func__, __FILE__, __LINE__);                      \
    // platform_halt();                                                                                                   \
    throw "unimplemented"
