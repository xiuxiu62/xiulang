#pragma once

#include "allocators.hpp"
#include "collections/shared.hpp"
#include "defines.hpp"
#include <cstring>

template <typename K> struct hash_set {
    struct entry {
        K key;
        bool occupied;
        bool deleted;
    };

    parent_allocator allocator = {};

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

template <typename K> static bool maybe_resize(hash_set<K> &hs);
template <typename K> static u32 find_bucket(const hash_set<K> &hs, const K &key, bool for_insertion = false);

template <typename K> bool hash_set<K>::init(arena_allocator &allocator, u32 initial_capacity) {
    if (initial_capacity == 0) initial_capacity = DEFAULT_CAPACITY;

    bucket_count = 1;
    while (bucket_count < initial_capacity) {
        bucket_count <<= 1;
    }

    buckets = (entry *)allocator.alloc(sizeof(entry) * bucket_count);
    if (!buckets) return false;
    this->allocator = {allocation_type::arena, &allocator};

    memset(buckets, 0, sizeof(entry) * bucket_count);
    size = 0;
    return true;
}

template <typename K> bool hash_set<K>::init(block_allocator &allocator, u32 initial_capacity) {
    if (initial_capacity == 0) initial_capacity = DEFAULT_CAPACITY;

    bucket_count = 1;
    while (bucket_count < initial_capacity) {
        bucket_count <<= 1;
    }

    buckets = (entry *)allocator.alloc(sizeof(entry) * bucket_count);
    if (!buckets) return false;
    this->allocator = {allocation_type::block, &allocator};

    memset(buckets, 0, sizeof(entry) * bucket_count);
    size = 0;
    return true;
}

template <typename K> void hash_set<K>::deinit() {
    if (buckets) {
        allocator.dealloc(buckets);
        buckets = nullptr;
        allocator = {};
    }
    bucket_count = 0;
    size = 0;
}

template <typename K> bool hash_set<K>::insert(const K &key) {
    if (!maybe_resize(*this)) return false;

    u32 bucket_index = find_bucket(*this, key, true);
    entry &bucket = buckets[bucket_index];

    if (!bucket.occupied || bucket.deleted) {
        if (!bucket.occupied) size++;
        bucket.key = key;
        bucket.occupied = true;
        bucket.deleted = false;
        return true;
    }

    return false;
}

template <typename K> bool hash_set<K>::contains(const K &key) const {
    u32 bucket_index = find_bucket(*this, key);
    const entry &bucket = buckets[bucket_index];
    return bucket.occupied && !bucket.deleted;
}

template <typename K> bool hash_set<K>::remove(const K &key) {
    u32 bucket_index = find_bucket(*this, key);
    entry &bucket = buckets[bucket_index];

    if (bucket.occupied && !bucket.deleted) {
        bucket.deleted = true;
        size--;
        return true;
    }
    return false;
}

template <typename K> void hash_set<K>::clear() {
    if (buckets) {
        memset(buckets, 0, sizeof(entry) * bucket_count);
    }
    size = 0;
}

// Helper functions
template <typename K> static bool maybe_resize(hash_set<K> &hs) {
    f32 load_factor = (f32)hs.size / (f32)hs.bucket_count;
    if (load_factor < hash_set<K>::MAX_LOD_FACTOR) return true;

    // Store old data
    typename hash_set<K>::entry *old_buckets = hs.buckets;
    u32 old_bucket_count = hs.bucket_count;

    // Double capacity
    hs.bucket_count *= 2;
    hs.buckets =
        (typename hash_set<K>::entry *)hs.allocator.alloc(sizeof(typename hash_set<K>::entry) * hs.bucket_count);
    if (!hs.buckets) {
        // Restore old state on failure
        hs.buckets = old_buckets;
        hs.bucket_count = old_bucket_count;
        return false;
    }

    memset(hs.buckets, 0, sizeof(typename hash_set<K>::entry) * hs.bucket_count);
    u32 old_size = hs.size;
    hs.size = 0;

    // Rehash all entries
    for (u32 i = 0; i < old_bucket_count; i++) {
        if (old_buckets[i].occupied && !old_buckets[i].deleted) {
            u32 new_bucket_index = find_bucket(hs, old_buckets[i].key, true);
            hs.buckets[new_bucket_index] = old_buckets[i];
            hs.buckets[new_bucket_index].deleted = false;
            hs.size++;
        }
    }

    // Clean up old buckets
    hs.allocator.dealloc(old_buckets);
    return true;
}

template <typename K> static u32 find_bucket(const hash_set<K> &hs, const K &key, bool for_insertion) {
    u32 hash_value = hash(key);
    u32 index = hash_value & (hs.bucket_count - 1); // Fast modulo for power of 2
    u32 original_index = index;
    u32 first_deleted = hs.bucket_count; // Invalid index

    do {
        const typename hash_set<K>::entry &bucket = hs.buckets[index];

        if (!bucket.occupied) {
            // Empty slot found
            if (for_insertion && first_deleted != hs.bucket_count) {
                return first_deleted; // Reuse deleted slot
            }
            return index;
        }

        if (bucket.deleted) {
            // Remember first deleted slot for insertion
            if (for_insertion && first_deleted == hs.bucket_count) {
                first_deleted = index;
            }
        } else if (bucket.key == key) {
            // Key found
            return index;
        }

        // Linear probing
        index = (index + 1) & (hs.bucket_count - 1);
    } while (index != original_index);

    // Table is full or we've wrapped around
    if (for_insertion && first_deleted != hs.bucket_count) {
        return first_deleted;
    }
    return index; // This shouldn't happen if resize works correctly
}

// Template instantiations
template struct hash_set<u32>;
template struct hash_set<u64>;
template struct hash_set<const char *>;
