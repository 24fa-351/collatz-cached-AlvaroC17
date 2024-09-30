#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cache.h"

#define TEST_MODE 0

int collatz_steps(unsigned long n) {
    int steps = 0;
    while (n != 1) {
        if (n % 2 == 0) {
            n /= 2;
        } else {
            n = 3 * n + 1;
        }
        steps++;
    }
    return steps;
}

void run_collatz(int N, unsigned long MIN, unsigned long MAX, CachePolicy policy, int cache_size) {
    Cache *cache = NULL;

    if (policy != NO_CACHE) {
        cache = cache_init(policy, cache_size);
    }

    srand(time(NULL));

    for (int i = 0; i < N; i++) {
        unsigned long RN = MIN + rand() % (MAX - MIN + 1);
        int steps;

        if (policy == NO_CACHE) {
            steps = collatz_steps(RN);  // Bypass cache 
        } else {
            steps = cache_get_steps(cache, RN);
        }

        printf("%lu,%d\n", RN, steps);
    }

    if (policy != NO_CACHE) {
        printf("Cache hit rate: %.2f%%\n", cache_hit_rate(cache) * 100);
        cache_free(cache);
    }
}


int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Usage: %s <N> <MIN> <MAX> <cache_policy>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    unsigned long MIN = strtoul(argv[2], NULL, 10);
    unsigned long MAX = strtoul(argv[3], NULL, 10);
    CachePolicy policy = atoi(argv[4]);  // 0 = None, 1 = LRU, 2 = FIFO
    int cache_size = 1000;

    run_collatz(N, MIN, MAX, policy, cache_size);
    return 0;
}
