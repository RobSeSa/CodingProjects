#ifndef __HASH_H__
#define __HASH_H__

#include "ll.h"
#include <inttypes.h>

//
// Struct definition for a HashTable.
//
// salt:   An array to store the HashTable's salt.
// length: The length of the HashTable, or how many entries it can support.
// head:   An array of linked list heads, which are ListNodes.
//
typedef struct HashTable {
  uint64_t salt[2];
  uint32_t length;
  ListNode **head;
} HashTable;

//
// Converts a code number into a string for hashing.
// Note that the string, if printed, isn't actually the code number.
//
// code_num:   The code number to convert into a string for hashing.
//
char *code_num_to_key(uint64_t code_num);

//
// Wrapper function to get hash value by using SPECK's key hash.
// SPECK's key hash requires a key and a salt.
//
// ht:     The hash table containing the salt for hashing.
// key:    The key to hash.
//
uint32_t hash(HashTable *ht, char *key);

//
// Creates a new hash table of specified length with a preset salt.
// Initializes hash table with all ASCII characters and their codes.
//
// length:     The specified length of the hash table.
//
HashTable *ht_create(uint64_t length);

//
// Frees memory allocated for a hash table.
//
// ht:   The hash table to free.
//
void ht_delete(HashTable *ht);

//
// Searches for a ListNode containing a code number in the hash table.
// Returns NULL if not found.
//
// ht:         The hash table to search in.
// code_num:   The code number to search for.
//
ListNode *ht_lookup(HashTable *ht, uint64_t code_num);

//
// Inserts a new entry into a HashTable.
// The key is a code number and the value is a word.
// Calls ll_add_code_num() to create a new ListNode for the new entry.
//
// code_num:   The code number for the entry.
// word:       The word for the entry.
// len:        The length of the entry's word.
//
void ht_add(HashTable *ht, uint64_t code_num, uint8_t *word, uint64_t len);

//
// Prints a hash table.
// Debug flag must by set to see anything.
//
// ht:  The hash table to print.
//
void ht_print(HashTable *ht);

#endif
