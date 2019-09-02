// main.c

#include "ll.h"
#include "bv.h"
#include "trie.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main() {
   printf("This is the main function of LZW\n");

   //ListNode testing
/*   char *word = "temp";
   ListNode *listNode = ll_node_create(98, (uint8_t *)word, 4);
   printf("Printing the contents of listNode\n");
   ll_node_print(listNode);
   ll_node_delete(listNode);
*/
   //BitVector testing
/*   printf("Creating BitVector\n");
   BitVector *bv = bv_create(10);
   uint64_t byte_length = byte_len(10);
   printf("bv's length = %ld\n", byte_length);
   printf("getbit 9\n");
   printf("%u\n",  bv_get_bit(bv, 9) );
   bv_set_bit(bv, 9);
   printf("getbit 9 after setting\n");
   printf("%u\n",  bv_get_bit(bv, 9) );
   bv_clr_bit(bv, 9);
   printf("getbit 9 after clearing\n");
   printf("%u\n",  bv_get_bit(bv, 9) );
   printf("Deleting the bv\n");
   bv_delete(bv);
*/
   //More BitVector testing
/*   BitVector *codebv = code_num_to_bv(5, 9);
   printf("contents of codebv(printed from LSB in bv->vector): ");
   bv_print(codebv);
   printf("\n");
   BitVector *temp = codebv;
   uint64_t code = 0;
   //infer says codebv unreachable after this point
   code = bv_to_code_num(codebv);
   printf("codebv->length = %u\n", codebv->length);
   printf("code = %lu\n", code);
   bv_delete(temp);
*/
   //TrieNode testing
   TrieNode *tn = trie_create();
   TrieNode *root = tn;
   printf("tn->sym = %u\ntn->code_num = %lu\n", tn->sym, tn->code_num);
   trie_print(tn, 0);
   printf("\nStepping through the tree\n");
   char *word = "word";
   int i = 0;
   while(tn != NULL) {
      trie_node_print(tn);
      printf("\n");
      printf("word[i] = %u\n", (uint8_t)word[i]);
      tn = trie_step(tn, (uint8_t)word[i]);
      i++;
   }
   trie_delete(root);
   return 0;
}
