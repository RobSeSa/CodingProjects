// trie.h - Contains the function declarations for the Trie ADT.

#ifndef __TRIE_H__
#define __TRIE_H__

#include <inttypes.h>
#include <stdbool.h>

typedef struct TrieNode TrieNode;
//
// Struct definition for a TrieNode.
//
// children:   An array of TrieNodes.
// code_num:   The TrieNode's code number.
// sym:        The TrieNode's symbol, or character.
//
struct TrieNode {
  TrieNode *children[256];
  uint64_t code_num;
  uint8_t sym;
};

//
// Constructor for a TrieNode.
//
// sym:    The TrieNode's symbol, or character.
// code:   The TrieNode's unique code.
//
TrieNode *trie_node_create(uint8_t sym, uint64_t code_num);

//
// Destructor for a TrieNode.
//
// n:  The TrieNode to be freed.
//
void trie_node_delete(TrieNode *n);

//
// Creates and initializes a new Trie.
// All ASCII characters are initially added as the root's children.
//
TrieNode *trie_create();

//
// Recursively frees memory allocated for an entire Trie.
//
// n:  The root of the sub-Trie to free (starts with actual root).
//
void trie_delete(TrieNode *n);

//
// Returns the TrieNode one step down the Trie for the next specified symbol.
// Returns NULL if the symbol doesn't exist.
//
//
TrieNode *trie_step(TrieNode *n, uint8_t sym);

//
// Prints a TrieNode.
//
// n:  The TrieNode to print.
//
void trie_node_print(TrieNode *n);

//
// Recursively prints an entire Trie.
//
// root:   The root of the sub-Trie to print.
// depth:  The depth of the TrieNode in the Trie.
//
void trie_print(TrieNode *root, int depth);

#endif
