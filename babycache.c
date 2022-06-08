#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* VERSION: 1.0
 *
 * Linear search seems slow.. oh, this isn't a hash table because we have
 * not hashed the keys. Linear search is O(n) and hash is O(1) since we can
 * a direct look-up of the hash-ed key
 *
 */


char *ADDRESS = "localhost";


typedef struct {
	char *key;
	char *value;
} Entry;

typedef struct {
	int count;
	int capacity;
	Entry *items;
} Table;


// search
char *linear_search(Table *ht, char *key);


// cruds
Entry *create_item(char *key, char *value);
void insert_item(Table *ht, Entry *item);
void remove_item(char key[]);

// prints
void print_logo();
void print_awaiting_connections();


void print_awaiting_connections() {
	printf("\tListening on localhost:6969\n");
	printf("\n");
}


char *linear_search(Table *ht, char *key) {
	int i;
	for (i=0; i<ht->count;i++) {
		if (strcmp(ht->items[i].key, key) == 0) {
			return ht->items[i].value;
		}
	}
	return NULL;
}


Entry *create_item(char *key, char *value) {
	Entry *item = (Entry *) malloc(sizeof(Entry));
	if (item == NULL) {
		exit(127);
	}
	item->key = key;
	item->value = value;
	return item;
}

void insert_item(Table *ht, Entry *item) {
	ht->items = (Entry *) realloc(ht->items, sizeof(Entry) * (ht->count + 1));
	if (ht->items == NULL) {
		exit(127);
	}
	ht->count++;
	ht->items[ht->count - 1] = *item;
	printf("inserted: %s %s\n", item->key, item->value);
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

int main(int argc, char *argv[]) {
	print_logo();
	print_awaiting_connections();
	Table *ht = (Table *) malloc(sizeof(Table));
	if (ht == NULL) {
		exit(127);
	}
	ht->count = 0;
	ht->capacity = 0;
	ht->items = NULL;
	insert_item(ht, create_item("jon", "doe"));
	insert_item(ht, create_item("jon1", "doe1"));
	insert_item(ht, create_item("jon2", "doe2"));
	printf("%s\n", linear_search(ht, "jon1"));
	free(ht->items);
	free(ht);
	return 0;
}