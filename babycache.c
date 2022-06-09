#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "print.h"
#include "hash.h"

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
 * Essentially, open addressing gets a bucket, and if it has an entry and the key
 * is different, it increments the pointer to find the next empty bucket.
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

#define TABLE_MAX_LOAD          0.75
#define DEBUG_PRINT_CODE

/* DATA STRUCTS */

typedef struct {
    char *key;
    char *value;
} Entry;

typedef struct {
    int count; 
    int capacity;
    Entry *entries;
} Table;

#define GROW_CAPACITY(capacity)  ((capacity) < 8 ? 8 : (capacity) * 2)

/* HEADERS */

void adjust_table_capacity(Table *ht, int capacity);

Table *table_create();
void table_init(Table *ht);
void table_free(Table *ht);
bool table_add(Table *ht, char *key, char *value);
bool table_get(Table *ht, char *key, char *value);

static Entry *find_entry(Entry *entries, int capacity, char *key);

void print_entries(Table *ht);

/* IMPLEMENTATION */

void print_entries(Table *ht) {
#ifdef DEBUG_PRINT_CODE
    for (int i=0; i<ht->capacity; i++) {
        printf("%d: %s->%s\n", i, ht->entries[i].key, ht->entries[i].value);
    }
#endif
}

static Entry *find_entry(Entry *entries, int capacity, char *key) {
    uint32_t index = hash_string(key) & (capacity - 1);

    Entry* tombstone = NULL;
  
    for (;;) {
        Entry* entry = &entries[index];
        if (entry->key == NULL) {
            if (entry->value == NULL) {
                return tombstone != NULL ? tombstone : entry;
            } else {
                if (tombstone == NULL) tombstone = entry;
            }
        } else if (entry->key == key) {
            return entry;
        }
        index = (index + 1) & (capacity - 1);
    }
}


void adjust_table_capacity(Table *ht, int capacity) {
    /* Adjust Capacity of Hash table 
     *
     * The bin size is too low and we can get conflicts.
     * This resizes allocates more memory for more bins
     * to prevent conflicts.
     *
     * 1. Allocate memory for new capacity size
     * 2. Set all buckets to NULL
     * 3. Move all entries in table to the new memory location
     * 4. Free and update table entries pointer
     **/
    Entry *entries = (Entry *) malloc(sizeof(Entry) * capacity);
    if (entries == NULL) exit_oom();
    for (int i=0; i < capacity; i++) {
        entries[i].key = NULL;
        entries[i].value = NULL;
    }

    ht->count = 0;
    for (int i=0; i < ht->capacity; i++) {
        Entry *entry = &ht->entries[i];
        if (entry->key == NULL) continue;

        Entry *dest = find_entry(entries, capacity, entry->key);
        dest->key = entry->key;
        dest->value = entry->value;
        ht->count++;
    }

    free(ht->entries);
    ht->entries = entries;
    ht->capacity = capacity;
}

bool table_add(Table *ht, char *key, char *value) {
    /* Add an entry into the table
     *
     * 1. Check capacity of table and grow it if the load is too high
     * 2. Find Entry (can return tombstone). Balancing requires 
     *    resetting all the buckets.
     *          Q: Why does capacity change before finding a hit?
     */

    // 1:
    if (ht->count + 1 > ht->capacity * TABLE_MAX_LOAD) {
        int capacity = GROW_CAPACITY(ht->capacity);
        adjust_table_capacity(ht, capacity); // if < 8, 8, else x*2
    }

    // 2:
    Entry *entry = find_entry(ht->entries, ht->capacity, key);
    bool isNewKey = entry->key == NULL;
    if (isNewKey && (entry->value == NULL)) ht->count++;

    entry->key = key;
    entry->value = value;

    return isNewKey;
}

void table_init(Table *ht) {
    ht->count = 0;
    ht->capacity = 0;
    ht->entries = NULL;
}

Table *table_create() {
    Table *ht = (Table *) malloc(sizeof(Table));
    if (ht == NULL) exit_oom();
    table_init(ht);
    return ht;
}

void table_free(Table *ht) {
    free(ht->entries);
    free(ht);
    table_init(ht);
}

int main(int argc, char *argv[]) {
    print_logo();
    print_awaiting_connections();

    Table *ht = table_create();

    table_add(ht, "jon", "doe1");
    table_add(ht, "jon1", "doe1");
    table_add(ht, "jon2", "doe2");
    table_add(ht, "jon3", "doe3");
    table_add(ht, "jon4", "doe4");
    table_add(ht, "jon5", "doe5");
    table_add(ht, "jon6", "doe6");

    print_entries(ht);

    table_free(ht);
    return 0;
}