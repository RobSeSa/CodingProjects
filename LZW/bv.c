#include "bv.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

uint64_t byte_len(uint64_t bit_len) {
   uint64_t byte_len = bit_len/8;
   if(bit_len%8 != 0) {
      byte_len++;
   }
   return byte_len;
}

BitVector *bv_create(uint8_t bit_len) {
   BitVector *n = malloc(sizeof(BitVector));
   assert(n);
   n->vector = (uint8_t*) calloc(byte_len(bit_len), sizeof(uint8_t));
   //initializing values to 0
   assert(n->vector);
   n->length = bit_len;
   return n;
}

void bv_delete(BitVector *v) {
   free(v->vector);
   free(v);
}

void bv_set_bit(BitVector *v, uint8_t i) {
   uint8_t index = i%8;
   uint8_t byte_index = i/8;
   v->vector[byte_index] |= 1U << index;
}

void bv_clr_bit(BitVector *v, uint8_t i) {
   uint8_t index = i%8;
   uint8_t byte_index = i/8;
   v->vector[byte_index] &= ~(1U << index);
}

uint8_t bv_get_bit(BitVector *v, uint8_t i) {
   uint8_t index = i%8;
   uint8_t byte_index = i/8;
   uint8_t bit = (v->vector[byte_index] >> index) & 1U;
   return bit;
   
}

BitVector *code_num_to_bv(uint64_t code_num, uint8_t bit_len) {
   BitVector *bv = bv_create(bit_len);
   uint8_t bit;
   //printf("code_num is %lu with length %u\n", code_num, bit_len);
   for(int i = 0; i < bit_len; i++) {
      bit = (code_num >> i) & 1UL;
      if(bit) { bv_set_bit(bv, i); }
   }
   printf("\n");

   return bv;
}

uint64_t bv_to_code_num(BitVector *v) {
   int i;
   uint8_t bit, byte_index, index;
   uint64_t code = 0;
   //printf("code_num is %lu with length %u\n", code_num, bit_len);
   for(i = v->length - 1; i >= 0; i--) {
      //printf("code: %lu\n", code);
      code = code << 1;
      byte_index = i/8;
      index = i%8;
      bit = (v->vector[byte_index] >> index) & 1UL;
      if(bit) { code++; }
   }
   return code;
}
void bv_print(BitVector *v) {
   int bit, byte_index, index;
   //printf("code_num is %lu with length %u\n", code_num, bit_len);
   for(byte_index = 0; byte_index < v->length / 8; byte_index += 8) {
      for(index = 7; index >= 0; index--) {
         bit = (v->vector[byte_index] >> index) & 1UL;
         printf("%u", bit);
      }
   }
}
/*
void bv_print(BitVector *v) {
   int i;
   uint8_t bit, byte_index, index;
   //printf("code_num is %lu with length %u\n", code_num, bit_len);
   for(i = 0; i < v->length; i++) {
      byte_index = i/8;
      index = i%8;
      bit = (v->vector[byte_index] >> index) & 1UL;
      printf("%u", bit);
   }
}
*/
