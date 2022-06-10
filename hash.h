#ifndef babycache_hash_h
#define babycache_hash_h

#define FNV_OFFSET      2166136261u
#define FNV_PRIME       16777619

uint64_t hash_string(const char* string);

#endif