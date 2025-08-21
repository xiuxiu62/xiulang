#pragma once

#include "allocators.hpp"
#include "defines.hpp"

template <typename K, typename V> struct hash_map {
    struct bucket {
        K key;
        V value;
        bool occupied;
        bool deleted;
    };

    parent_allocator allocator = {};

    bucket *buckets;
    u32 bucket_count;
    u32 size;

    static constexpr u32 DEFAULT_CAPACITY = 16;
    static constexpr f32 MAX_LOD_FACTOR = 0.7f;

    bool init(arena_allocator &allocator, u32 initial_capacity = DEFAULT_CAPACITY);
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

// template <typename V> struct hash_map<u32, V>;
// template <typename V> struct hash_map<u64, V>;
// template <typename V> struct hash_map<const char *, V>;

template <typename K, typename V> static bool maybe_resize(hash_map<K, V> &hm);
template <typename K, typename V>
static u32 find_bucket(const hash_map<K, V> &hm, const K &key, bool for_insertion = false);

template <typename K, typename V> bool hash_map<K, V>::init(arena_allocator &allocator, u32 initial_capacity) {
    if (initial_capacity == 0) initial_capacity = DEFAULT_CAPACITY;

    bucket_count = 1;
    while (bucket_count < initial_capacity) {
        bucket_count <<= 1;
    }

    buckets = (bucket *)allocator.alloc(sizeof(bucket) * bucket_count);
    if (!buckets) return false;
    this->allocator = {allocation_type::arena, &allocator};

    memset(buckets, 0, sizeof(bucket) * bucket_count);
    size = 0;
    return true;
}

template <typename K, typename V> bool hash_map<K, V>::init(block_allocator &allocator, u32 initial_capacity) {
    if (initial_capacity == 0) initial_capacity = DEFAULT_CAPACITY;

    bucket_count = 1;
    while (bucket_count < initial_capacity) {
        bucket_count <<= 1;
    }

    buckets = (bucket *)allocator.alloc(sizeof(bucket) * bucket_count);
    if (!buckets) return false;
    this->allocator = {allocation_type::block, &allocator};

    memset(buckets, 0, sizeof(bucket) * bucket_count);
    size = 0;
    return true;
}

template <typename K, typename V> void hash_map<K, V>::deinit() {
    if (buckets) {
        allocator.dealloc(buckets);
        buckets = nullptr;
        allocator = {};
    }
    bucket_count = 0;
    size = 0;
}

template <typename K, typename V> bool hash_map<K, V>::insert(const K &key, const V &value) {
    if (!maybe_resize(*this)) return false;

    u32 bucket_index = find_bucket(*this, key, true);
    bucket &b = buckets[bucket_index];

    if (!b.occupied || b.deleted) {
        if (!b.occupied) size++;
        b.key = key;
        b.value = value;
        b.occupied = true;
        b.deleted = false;
        return true;
    } else {
        // Key already exists, update value
        b.value = value;
        return true;
    }
}

template <typename K, typename V> V *hash_map<K, V>::get(const K &key) {
    u32 bucket_index = find_bucket(*this, key);
    bucket &b = buckets[bucket_index];
    if (b.occupied && !b.deleted) {
        return &b.value;
    }
    return nullptr;
}

template <typename K, typename V> const V *hash_map<K, V>::get(const K &key) const {
    u32 bucket_index = find_bucket(*this, key);
    const bucket &b = buckets[bucket_index];
    if (b.occupied && !b.deleted) {
        return &b.value;
    }
    return nullptr;
}

template <typename K, typename V> bool hash_map<K, V>::contains(const K &key) const {
    u32 bucket_index = find_bucket(*this, key);
    const bucket &b = buckets[bucket_index];
    return b.occupied && !b.deleted;
}

template <typename K, typename V> bool hash_map<K, V>::remove(const K &key) {
    u32 bucket_index = find_bucket(*this, key);
    bucket &b = buckets[bucket_index];

    if (b.occupied && !b.deleted) {
        b.deleted = true;
        size--;
        return true;
    }
    return false;
}

template <typename K, typename V> void hash_map<K, V>::clear() {
    if (buckets) {
        memset(buckets, 0, sizeof(bucket) * bucket_count);
    }
    size = 0;
}

template <typename K, typename V> V &hash_map<K, V>::operator[](const K &key) {
    u32 bucket_index = find_bucket(*this, key, true);
    bucket &b = buckets[bucket_index];

    if (!b.occupied || b.deleted) {
        // Need to insert a new entry
        if (!maybe_resize(*this)) {
            // If resize fails, we still need to return something
            // This is a bit dangerous, but matches typical std::map behavior
            static V default_value{};
            return default_value;
        }

        // Recalculate bucket index after potential resize
        bucket_index = find_bucket(*this, key, true);
        bucket &new_b = buckets[bucket_index];

        if (!new_b.occupied) size++;
        new_b.key = key;
        new_b.value = V{}; // Default construct value
        new_b.occupied = true;
        new_b.deleted = false;
        return new_b.value;
    }

    return b.value;
}

// Helper functions
template <typename K, typename V> static bool maybe_resize(hash_map<K, V> &hm) {
    f32 load_factor = (f32)hm.size / (f32)hm.bucket_count;
    if (load_factor < hash_map<K, V>::MAX_LOD_FACTOR) return true;

    // Store old data
    typename hash_map<K, V>::bucket *old_buckets = hm.buckets;
    u32 old_bucket_count = hm.bucket_count;

    // Double capacity
    hm.bucket_count *= 2;
    hm.buckets = (typename hash_map<K, V>::bucket *)hm.allocator.alloc(sizeof(typename hash_map<K, V>::bucket) *
                                                                       hm.bucket_count);
    if (!hm.buckets) {
        // Restore old state on failure
        hm.buckets = old_buckets;
        hm.bucket_count = old_bucket_count;
        return false;
    }

    memset(hm.buckets, 0, sizeof(typename hash_map<K, V>::bucket) * hm.bucket_count);
    u32 old_size = hm.size;
    hm.size = 0;

    // Rehash all entries
    for (u32 i = 0; i < old_bucket_count; i++) {
        if (old_buckets[i].occupied && !old_buckets[i].deleted) {
            u32 new_bucket_index = find_bucket(hm, old_buckets[i].key, true);
            hm.buckets[new_bucket_index] = old_buckets[i];
            hm.buckets[new_bucket_index].deleted = false;
            hm.size++;
        }
    }

    // Clean up old buckets
    hm.allocator.dealloc(old_buckets);
    return true;
}

template <typename K, typename V> static u32 find_bucket(const hash_map<K, V> &hm, const K &key, bool for_insertion) {
    u32 hash_value = hash(key);
    u32 index = hash_value & (hm.bucket_count - 1); // Fast modulo for power of 2
    u32 original_index = index;
    u32 first_deleted = hm.bucket_count; // Invalid index

    do {
        const typename hash_map<K, V>::bucket &bucket = hm.buckets[index];

        if (!bucket.occupied) {
            // Empty slot found
            if (for_insertion && first_deleted != hm.bucket_count) {
                return first_deleted; // Reuse deleted slot
            }
            return index;
        }

        if (bucket.deleted) {
            // Remember first deleted slot for insertion
            if (for_insertion && first_deleted == hm.bucket_count) {
                first_deleted = index;
            }
        } else if (bucket.key == key) {
            // Key found
            return index;
        }

        // Linear probing
        index = (index + 1) & (hm.bucket_count - 1);
    } while (index != original_index);

    // Table is full or we've wrapped around
    if (for_insertion && first_deleted != hm.bucket_count) {
        return first_deleted;
    }
    return index; // This shouldn't happen if resize works correctly
}
