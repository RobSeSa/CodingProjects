#include "hash.h"
#include "io.h"
#include "ll.h"
#include "speck.h"
#include <inttypes.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

char *code_num_to_key(uint64_t code_num) {
   char *temp = (char *)malloc(sizeof(uint8_t) * 8);

//   printf("code_num_to_key(%lu)\n", code_num);
   int i;
   for(i = 7; i >= 0; i--) {
      temp[i] = (char)(code_num >> ((8 - (i+1)) * 8));
//      printf("temp[%d] = %u\n", i, temp[i]);
   }
   return temp;
}

uint64_t hash(HashTable *ht, uint64_t key) {
   //convert uint64_t to char *
   char *temp = code_num_to_key(key);
   //call speck operation of key
   //set length to 8 bc length of temp in code_num_to_key is always 8
   return keyed_hash(temp, 8, ht->salt);
}

HashTable *ht_create(uint64_t length) {
   HashTable *temp = malloc(sizeof(HashTable));
   assert(temp);
   //unsure what to use as salt
   temp->salt[0] = MAGIC;
   temp->salt[1] = MAGIC;
   temp->length = length;
   temp->head = malloc(sizeof(ListNode)*length);
   return temp;
}

void ht_delete(HashTable *ht) {
   free(ht->head);
   free(ht);   
}

ListNode *ht_lookup(HashTable *ht, uint64_t code_num) {
   ListNode *temp = ll_lookup(&ht->head[hash(ht, code_num) % ht->length], code_num);
   if(temp != NULL) { return temp; }
   return NULL;
}

void ht_add(HashTable *ht, uint64_t code_num, uint8_t *word, uint64_t len) {
   ht->head[hash(ht, code_num) % ht->length] = 
      ll_add(&ht->head[hash(ht, code_num) % ht->length], code_num, word, len);
}
void ht_print(HashTable *ht) {
   printf("Printing hash table\n");
   printf("salt[0] = %lu\n", ht->salt[0]);
   printf("salt[1] = %lu\n", ht->salt[1]);
   printf("length = %u\n", ht->length);
   int i;
   for(i = 0; i < 256; i++) {
      printf("head[%d]: ", i);
      ll_print(ht->head[i]);
      printf("\n");
   }
}
