#include "collections.hpp"
#include "memory.hpp"
#include <vcruntime_string.h>

// template <typename K> struct hash_set {
//     struct entry {
//         K key;
//         bool occupied;
//         bool deleted;
//     };

//     entry *buckets;
//     u32 bucket_count;
//     u32 size;

template <typename K> bool hash_set<K>::init(arena &allocator, u32 initial_capacity) {

    if (initial_capacity == 0) initial_capacity = DEFAULT_CAPACITY;

    bucket_count = 1;
    while (bucket_count < initial_capacity) {
        bucket_count <<= 1;
    }

    buckets = (entry *)alloc(allocator, sizeof(entry) * bucket_count);
    if (!buckets) return false;
    this->allocator = &allocator;

    memset(buckets, 0, sizeof(entry) * bucket_count);
    size = 0;
    return true;
}

template <typename K> void hash_set<K>::deinit() {
    if (buckets) {
        buckets = nullptr;
        allocator = nullptr;
    }
    bucket_count = 0;
    size = 0;
}

template <typename K> bool hash_set<K>::insert(const K &key) {
}

template <typename K> bool hash_set<K>::contains(const K &key) const {
}

template <typename K> bool hash_set<K>::remove(const K &key) {
}

template <typename K> void hash_set<K>::clear() {
}
