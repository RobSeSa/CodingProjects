// main.c

#include "ll.h"
#include "bv.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main() {
   printf("This is the main function of LZW\n");
   char *word = "temp";
   ListNode *listNode = ll_node_create(98, (uint8_t *)word, 4);
   printf("Printing the contents of listNode\n");
   ll_node_print(listNode);
   ll_node_delete(listNode);
   //done with listnode
   printf("Creating BitVector\n");
   BitVector *bv = bv_create(10);
   uint64_t byte_length = byte_len(10);
   printf("bv's length = %ld\n", byte_length);
   printf("Deleting the bv\n");
   bv_delete(bv);
   //done with bv
   printf("\nTesting bit operations\n");
   printf("1UL = %lu\n", 1UL);
   printf("1UL << 1 = %lu\n", (1UL << 1));
   return 0;
}
