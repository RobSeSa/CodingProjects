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
   v->vector[byte_index] |= 1UL << index;
}

void bv_clr_bit(BitVector *v, uint8_t i) {
   uint8_t index = i%8;
   uint8_t byte_index = i/8;
   v->vector[byte_index] &= ~(1UL << index);
}
