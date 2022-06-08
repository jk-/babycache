#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* VERSION: 1.0
 * --------------------------
 * This isn't a hash table because we have not hashed the keys. 
 * Linear search is O(n) and hash is O(1) at best since we can
 * do a direct look-up of the hash-ed key.
 *
 */

/* VERSION: 2.0
 * --------------------------
 * Implement hash on the Entry. Use the look-up to find by hash value.
 * Still haven't thought about capacity, conflict mitigation, inserting, etc.
 *
 * -- LEARNING LOG --
 * This is inductive - im not looking things up, just thinking until I get stumped.
 *
 * I renamed things to make it more clear and added helper methods for memory, etc.
 *
 * I looked up hashing algos and FNV-1a looks to be the best aglo for performance
 * and conflict. There is a concept called table load which is the ratio of
 * count to capacity for conflicts (typical of any hashing algo and conflicts).
 *
 * Original doing linear search, I set the Table->count as the index of the entries
 *      ht->entries[count] = *item;
 * This means I can't look up a key without a linear search. 
 * Also, if I can't put the hashkey as the index because well, it would segfault.
 *
 *      Q: I think the next idea is to bucket the index? Take some mod of the hash to 
 *         see what bucket it fits in. Then, once we have the bucket, it needs to be 
 *         a list of entries for clashes? Also - how does that work allocating a 
 *         dynamic size of entries?
 *
 *      A: This is called "Separate Chaining" - this means that eventually each
 *         bucket will resemble an unsorted linked list. O(n) lookup; not good enough.
 *
 * Q: What next? Random buckets?
 *
 * "Seperate chaining" brought me to "Open Addressing" --
 *      Q: My main question now is what if you run out of empty buckets?
 *      A: when we add a new entry, we grow the hash table relative to its load 
 *      (probability of conflict) or "capacity".
 *
 * Im not going to store the hash - im going to hash the key on the fly and mod it.
 * to look up the bucket location. If the key in that bucket is the same, I have a hit
 * on a read. if its not, I have to increment ("probe sequencing") until I run out of
 * indexes (this is the worst case scenario).
 *
 */

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

/* HEADERS */

typedef struct {
    char *key;
    char *value;
} Entry;

typedef struct {
    int count; 
    int capacity;   // now we know why capacity is here
    Entry *entries;
} Table;


static uint64_t hash_string(const char* string);


Table *table_init();
void table_free(Table *ht);
void table_insert(Table *ht, char *key, char *value);
char *table_get(Table *ht, char *key, uint32_t hash);

Entry *create_entry(char *key, char *value);

void print_logo();
void print_awaiting_connections();

int exit_oom();


/* IMPLEMENTATION */

int exit_oom() {
    // Unable to allocate memory
    printf("Unable to allocate memory.");
    exit(137);  // Out of memory
}

void print_awaiting_connections() {
    printf("\tListening on localhost:6969\n");
    printf("\n");
}

Entry *create_entry(char *key, char *value) {
    Entry *entry = (Entry *) malloc(sizeof(Entry));
    if (entry == NULL)
        exit_oom();
    entry->key = key;
    entry->value = value;
    return entry;
}

static uint64_t hash_string(const char* string) {
    uint64_t hash = FNV_OFFSET;
    for (const char* p = string; *p; p++) {
        hash ^= (uint64_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}

void table_add(Table *ht, Entry *entry) {
    // TODO:
    // We need to find the key in the table before we allocate more space
    // Since we are now going to bin the hash keys.
    // We need to change the interface to accept key/value and not an entire entry
    // No point in allocating an entries memory if its not added (can just replace
    // the value if the key exists
    ht->entries = (Entry *) realloc(ht->entries, sizeof(Entry) * (ht->count + 1));
    if (ht->entries == NULL)
        exit_oom();
    ht->entries[ht->count] = *entry;
    ht->count++;
    printf("inserted: %s %s\n", entry->key, entry->value);
}

void print_logo() {
    printf("\n");
    printf("\t _           _                          _\n");
    printf("\t| |__   __ _| |__  _   _  ___ __ _  ___| |__   ___\n");
    printf("\t| '_ \\ / _` | '_ \\| | | |/ __/ _` |/ __| '_ \\ / _ \\\n");
    printf("\t| |_) | (_| | |_) | |_| | (_| (_| | (__| | | |  __/\n");
    printf("\t|_.__/ \\__,_|_.__/ \\__, |\\___\\__,_|\\___|_| |_|\\___|\n");
    printf("\t                   |___/\n");
    printf("\n");
}

Table *table_init() {
    Table *ht = (Table *) malloc(sizeof(Table));
    if (ht == NULL)
        exit_oom();
    ht->count = 0;
    ht->capacity = 0;
    ht->entries = NULL;
    return ht;
}

void table_free(Table *ht) {
    free(ht->entries);
    free(ht);
}

int main(int argc, char *argv[]) {
    print_logo();
    print_awaiting_connections();

    Table *ht = table_init();

    table_add(ht, create_entry("jon", "doe"));
    table_add(ht, create_entry("jon1", "doe1"));
    table_add(ht, create_entry("jon2", "doe2"));

    table_free(ht);
    return 0;
}