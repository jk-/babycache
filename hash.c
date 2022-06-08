#include <stdlib.h>
#include "hash.h"

uint64_t hash_string(const char* string) {
    uint64_t hash = FNV_OFFSET;
    for (const char* p = string; *p; p++) {
        hash ^= (uint64_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}
