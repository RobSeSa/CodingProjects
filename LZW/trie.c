#include <inttypes.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "trie.h"

TrieNode *trie_node_create(uint8_t sym, uint64_t code_num) {
   TrieNode *tn = malloc(sizeof(TrieNode));
   assert(tn);
   tn->code_num = code_num;
   tn->sym = sym;
   return tn;
}

void trie_node_delete(TrieNode *n) {
   free(n);
}

TrieNode *trie_create() {
   TrieNode *tn = trie_node_create(0, 0); //root
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
TrieNode *trie_step(TrieNode *n, uint8_t sym) {
   if(n->children[sym] != NULL) { return n->children[sym]; }
   else { return NULL; }
}
void trie_node_print(TrieNode *n) {
   if((n->sym >= 32) && (n->sym <= 126)) { printf("(%c)", n->sym); }
   printf("%u,%lu", n->sym, n->code_num);
   
}
void trie_print(TrieNode *root, int depth) {
   //print self
   if(depth == 0) { printf("Printing Trie... (depth):(symbol),(code)\n"); }
   printf("%d:", depth);
   trie_node_print(root);
   printf("; ");
   int i;
   for(i = 0; i < 256; i++) {
      if(root->children[i] != NULL) {
         trie_print(root->children[i], depth + 1);
      }
   }
}
