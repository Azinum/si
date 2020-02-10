// hash.h

#ifndef _HASH_H
#define _HASH_H

#define HTABLE_KEY_SIZE 32

typedef char Hkey[HTABLE_KEY_SIZE];
typedef int Hvalue;
struct Item;

typedef struct {
	struct Item* items;
	unsigned int count;	// Count of used slots
	unsigned int size;	// Total size of the hash table
} Htable;

// Important!
// It's recommended that the size is a prime number
// This will result in less key collisions
Htable ht_create(unsigned int size);

unsigned int ht_insert_element(Htable* table, const Hkey key, const Hvalue value);

const Hvalue* ht_lookup(const Htable* table, const Hkey key);

int ht_element_exists(const Htable* table, const Hkey key);

void ht_remove_element(Htable* table, const Hkey key);

unsigned int ht_get_size(const Htable* table);

unsigned int ht_num_elements(const Htable* table);

void ht_free(Htable* table);

#endif