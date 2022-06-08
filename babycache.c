#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* VERSION: 1.0
 * --------------------------
 * Linear search seems slow.. oh, this isn't a hash table because we have
 * not hashed the keys. Linear search is O(n) and hash is O(1) since we can
 * do a direct look-up of the hash-ed key.
 *
 */

/* VERSION: 2.0
 * --------------------------
 * Implement hash on the Entry. Use the look-up to find by hash value.
 * Still haven't thought about capacity, conflict mitigation, inserting, etc.
 *
 * I renamed things to make it more clear and added helper methods for memory, etc.
 *
 */


typedef struct {
	char *key;
	uint32_t hash; // added a hashed key
	char *value;
} Entry;

typedef struct {
	int count;
	int capacity;
	Entry *entries;
} Table;


Table *table_init();
void table_free(Table *ht);
void table_insert(Table *ht, char *key, char *value);
char *table_get(Table *ht, char *key, uint32_t hash);

Entry *create_entry(char *key, char *value);

// prints
void print_logo();
void print_awaiting_connections();

// errors
int exit_oom();


//

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

void table_add(Table *ht, Entry *entry) {
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
	printf("hash table memory freed");
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