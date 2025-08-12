#pragma once

#include "ast.hpp"
#include "defines.hpp"

enum class target_arch : u32 {
    X86_64,
    AARCH64,
};

enum class target_platform : u32 {
    LINUX,
    WINDOWS,
    MACOS,
};

enum class target_format : u32 {
    ELF,
    PE,
    MACH_O,
};

struct compilation_target {
    target_arch arch;
    target_platform platform;
    target_format format;

    bool debug_symbols;
    bool optimize;
    u32 stack_alignment; // usually 8 or 16 bytes
};
