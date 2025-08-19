#pragma once

#include "allocators.hpp"
#include "defines.hpp"

struct pool_handle {
    u32 index;

    static constexpr pool_handle invalid() {
        return {u32_max};
    }

    constexpr bool operator==(pool_handle other) const {
        return index == other.index;
    }

    constexpr bool operator!=(pool_handle other) const {
        return index != other.index;
    }

    constexpr bool is_valid() const {
        return *this == pool_handle::invalid();
    }

    constexpr bool is_invalid() const {
        return *this == pool_handle::invalid();
    }
};

template <typename T> struct pool {
    block_allocator *allocator;
    T *data = nullptr;
    u32 size = 0;
    u32 capacity = 0;

    bool init(block_allocator &allocator, u32 initial_capacity = 0);
    void deinit();

    pool_handle push(T item);
    T *get(pool_handle handle);
    const T *get(pool_handle handle) const;
};

template <typename T> u32 hash(const T &key);
template <> u32 hash<u32>(const u32 &key);
template <> u32 hash<u64>(const u64 &key);
template <> u32 hash<usize>(const usize &key);
template <> u32 hash<i32>(const i32 &key);
template <> u32 hash<i64>(const i64 &key);
template <> u32 hash<isize>(const isize &key);
template <> u32 hash<const char *>(const char *const &key);

template <typename K> struct hash_set {
    struct entry {
        K key;
        bool occupied;
        bool deleted;
    };

    block_allocator *allocator;

    entry *buckets;
    u32 bucket_count;
    u32 size;

    static constexpr u32 DEFAULT_CAPACITY = 16;
    static constexpr f32 MAX_LOD_FACTOR = 0.7f;

    bool init(arena_allocator &allocator, u32 initial_capacity = DEFAULT_CAPACITY);
    bool init(block_allocator &allocator, u32 initial_capacity = DEFAULT_CAPACITY);
    void deinit();

    bool insert(const K &key);
    bool contains(const K &key) const;
    bool remove(const K &key);
    void clear();
};

template struct hash_set<u32>;
template struct hash_set<u64>;
template struct hash_set<const char *>;

template <typename K, typename V> struct hash_map {
    struct bucket {
        K key;
        V value;
        bool occupied;
        bool deleted;
    };

    bucket *buckets;
    u32 bucket_count;
    u32 size;

    static constexpr u32 DEFAULT_CAPACITY = 16;
    static constexpr f32 MAX_LOD_FACTOR = 0.7f;

    bool init(block_allocator &allocator, u32 initial_capacity = DEFAULT_CAPACITY);
    void deinit();

    bool insert(const K &key, const V &value);
    V *get(const K &key);
    const V *get(const K &key) const;
    bool contains(const K &key) const;
    bool remove(const K &key);
    void clear();

    V &operator[](const K &key);
};

template <typename V> struct hash_map<u32, V>;
template <typename V> struct hash_map<u64, V>;
template <typename V> struct hash_map<const char *, V>;
