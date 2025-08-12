#pragma once

#include "stdint.h"

#ifdef DEBUG
#define ENVIRONMENT_DEBUG
#define when_debug(...) __VA_ARGS__
#define when_debug_else(debug_code, release_code) debug_code
#else
#define ENVIRONMENT_RELEASE
#define when_debug(...)
#define when_debug_else(debug_code, release_code) release_code
#endif

#ifdef PLATFORM_WIN
#define FLUX_EXPORT __declspec(dllexport)
#define FLUX_IMPORT __declspec(dllimport)
#else
#define FLUX_EXPORT __attribute__((visibility("default")))
#define FLUX_IMPORT __attribute__((visibility("default")))
#endif

#define ALLOCATION_TRACKING
#define ENABLE_I18N

#ifdef ALLOCATION_TRACKING
#define when_allocation_tracking(...) __VA_ARGS__
#define when_allocation_tracking_else(debug_code, release_code) debug_code
#else
#define when_allocation_tracking(...)
#define when_allocation_tracking_else(debug_code, release_code) release_code
#endif

#define kilobytes(SIZE) 1024 * (SIZE)
#define megabytes(SIZE) 1024 * 1024 * (SIZE)
#define gigabytes(SIZE) 1024 * 1024 * 1024 * (SIZE)

#define PERSISTENT_STORAGE_MEMORY mb(64)
#define TEMPORARY_STORAGE_MEMORY mb(8)
#define FRAME_STORAGE_MEMORY kb(64)

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using isize = intptr_t;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using usize = uintptr_t;

using f32 = float;
using f64 = double;

using version = u16[3];

#define i8_max 127
#define i8_min (-128)
#define i16_max 32767
#define i16_min (-32768)
#define i32_max 2147483647
#define i32_min (-2147483648)
#define i64_max 9223372036854775807LL
#define i64_min (-9223372036854775808LL)
#define isize_max 9223372036854775807LL
#define isize_min (-9223372036854775808LL)

#define u8_max 255U
#define u16_max 65535U
#define u32_max 4294967295U
#define u64_max 18446744073709551615ULL
#define usize_max 18446744073709551615ULL

#define f32_max 3.40282347e+38f
#define f32_min (-3.40282347e+38f)
#define f32_epsilon 1.19209290e-07f

#define f64_max 1.7976931348623157e+308
#define f64_min (-1.7976931348623157e+308)
#define f64_epsilon 2.2204460492503131e-16
