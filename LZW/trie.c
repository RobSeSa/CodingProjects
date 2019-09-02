#include <inttypes.h>
#include "trie.h"

/*struct TrieNode {
  TrieNode *children[256];
  uint64_t code_num;
  uint8_t sym;
};*/

TrieNode *trie_node_create(uint8_t sym, uint64_t code_num) {
   TrieNode *tn = malloc(sizeof(TrieNode));
   assert(tn);
   TrieNode->code_num = code_num;
   TrieNode->sym = sym;
   return tn;
}

void trie_node_delete(TrieNode *n) {
   free(n);
}

TrieNode *trie_create() {
   TrieNode *tn = trie_node_create(NULL, NULL); //root
   int i;
   for(i = 0; i < 256; i++) {
      tn->children[i] = trie_node_create(i, i);
   }
   return tn;
}

void trie_delete(TrieNode *n) {
   //check all children if null
   int i;
   for(i = 0; i < 256; i++) {
      if(n->children[i] != NULL) {
         //call trie_delete on child if not null
         trie_delete(n->children[i]);
      }
   }
   //child should delete self and return if all children are now null
   trie_node_delete(n);
}
/*
TrieNode *trie_step(TrieNode *n, uint8_t sym);
void trie_node_print(TrieNode *n);
void trie_print(TrieNode *root, int depth);
