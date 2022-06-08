#ifndef babycache_hash_h
#define babycache_hash_h

#define FNV_OFFSET      14695981039346656037UL
#define FNV_PRIME       1099511628211UL

uint64_t hash_string(const char* string);

#endif