#include <stdio.h>
#include <stdlib.h>
#include "cache.h"

typedef struct CacheNode {
    unsigned long key;
    int value;
    struct CacheNode *next;
} CacheNode;

struct Cache {
    CachePolicy policy;
    int capacity;
    int size;
    int hits;
    int accesses;
    CacheNode **table;
};

int collatz_steps(unsigned long n);

CacheNode *cache_node_new(unsigned long key, int value) {
    CacheNode *node = (CacheNode *) malloc(sizeof(CacheNode));
    node->key = key;
    node->value = value;
    node->next = NULL;
    return node;
}

Cache *cache_init(CachePolicy policy, int capacity) {
    Cache *cache = (Cache *) malloc(sizeof(Cache));
    cache->policy = policy;
    cache->capacity = capacity;
    cache->size = 0;
    cache->hits = 0;
    cache->accesses = 0;
    cache->table = (CacheNode **) calloc(capacity, sizeof(CacheNode *));
    return cache;
}

void cache_free(Cache *cache) {
    for (int i = 0; i < cache->capacity; i++) {
        CacheNode *node = cache->table[i];
        while (node) {
            CacheNode *temp = node;
            node = node->next;
            free(temp);
        }
    }
    free(cache->table);
    free(cache);
}

unsigned long hash_key(unsigned long key, int capacity) {
    return key % capacity;
}


int cache_get_steps(Cache *cache, unsigned long n) {
    if (cache->policy == NO_CACHE) {
        return collatz_steps(n); 
    }
    
    // LRU or FIFO
    cache->accesses++;
    unsigned long idx = hash_key(n, cache->capacity);
    CacheNode *node = cache->table[idx];
    
    // Search for the value in the cache
    while (node) {
        if (node->key == n) {
            cache->hits++;
            return node->value;
        }
        node = node->next;
    }

    // Calculate steps if not found in cache
    int steps = collatz_steps(n);

    // Add to cache if using LRU or FIFO
    CacheNode *new_node = cache_node_new(n, steps);
    new_node->next = cache->table[idx];
    cache->table[idx] = new_node;
    cache->size++;

    return steps;
}


double cache_hit_rate(Cache *cache) {
    if (cache->accesses == 0) return 0.0;
    return (double)cache->hits / cache->accesses;
}
