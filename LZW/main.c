// main.c

#include "ll.h"
#include "bv.h"
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
   BitVector *codebv = code_num_to_bv(5, 9);
   printf("contents of codebv(printed from LSB in bv->vector): ");
   bv_print(codebv);
   printf("\n");
   BitVector *temp = codebv;
   uint64_t code;
   code = bv_to_code_num(codebv);
   printf("code = %lu\n", code);
   bv_delete(temp);

   //bit operation testing
/*   printf("\nTesting bit operations\n");
   printf("1UL = %lu\n", 1UL);
   printf("1UL << 1 = %lu\n", (1UL << 1));
   printf("1U = %u\n", 1U);
*/
   return 0;
}
