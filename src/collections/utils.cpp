#include "collections.hpp"

template <> u32 hash<u32>(const u32 &key) {
    u32 h = key;
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;
    return h;
}

template <> u32 hash<u64>(const u64 &key) {
    return hash<u32>((u32)key) ^ hash<u32>((u32)(key >> 32));
}

template <> u32 hash<i32>(const i32 &key) {
    return hash<u32>((u32)key);
}

template <> u32 hash<i64>(const i64 &key) {
    return hash<u64>((u64)key);
}

template <> u32 hash<const char *>(const char *const &key) {
    // FNV-1a
    u32 h = 2166136261U;
    for (const char *p = key; *p; ++p) {
        h ^= (u32)*p;
        h *= 16777619U;
    }
    return h;
}
