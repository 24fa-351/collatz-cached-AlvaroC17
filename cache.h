#ifndef CACHE_H
#define CACHE_H

typedef enum {
    NO_CACHE = 0,
    LRU_CACHE,
    FIFO_CACHE
} CachePolicy;

typedef struct Cache Cache;

Cache *cache_init(CachePolicy policy, int size);
void cache_free(Cache *cache);
int cache_get_steps(Cache *cache, unsigned long n);
double cache_hit_rate(Cache *cache);

#endif
