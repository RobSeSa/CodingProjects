// bv.c - Contains the implementation of the Bit Vector ADT.
  
#include "bv.h"
#include "util.h"
#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//
// Struct definition for a BitVector.
//
// vector:     The vector of bytes for bit operations.
// length:     The length in bits of the BitVector.
//
typedef struct BitVector {
  uint8_t *vector;
  uint8_t length;
} BitVector;

//
// Returns number of bytes needed to represent the bit length.
//
// bit_len:    The length in bits.
//
uint64_t byte_len(uint64_t bit_len) {
   uint64_t byte_len = bit_len/8;
   if(bit_len%8 != 0) {
      byte_len++;
   }
   return byte_len;
}

//
// Creates a new BitVector of specified length.
//
// bit_len:    The length in bits of the BitVector.
//
BitVector *bv_create(uint8_t bit_len) {
   BitVector *n = malloc(sizeof(BitVector));
   assert(n);
   n->vector = (uint8_t*) calloc(byte_len(bit_len), sizeof(uint8_t));
   //initializing values to 0
   assert(n->vector);
   n->length = bit_len;
   return n;
}

//
// Frees memory allocated for a BitVector.
//
// v:  The BitVector to free memory for.
//
void bv_delete(BitVector *v) {
   free(v->vector);
   free(v);
}

//
// Sets a specified bit in a BitVector.
//
// v:  The BitVector.
// i:  Index of the bit to set.
//
void bv_set_bit(BitVector *v, uint8_t i);

//
// Clears a specified bit in a BitVector.
//
// v:  The BitVector.
// i:  Index of the bit to set.
//
void bv_clr_bit(BitVector *v, uint8_t i);

//
// Gets a specified bit in a BitVector.
//
// v:  The BitVector.
// i:  Index of the bit to get.
//
uint8_t bv_get_bit(BitVector *v, uint8_t i);

//
// Converts a code number into a new BitVector.
// Bits are set from the end of the BitVector since codes are right-aligned.
//
// code_num:   The code number to convert into a BitVector.
//
BitVector *code_num_to_bv(uint64_t code_num, uint8_t bit_len);

//
// Converts a BitVector into a code number.
// Bits are summed from the end since codes are right-aligned.
//
// v:  The BitVector to convert into a code number.
//
uint64_t bv_to_code_num(BitVector *v);

//
// Prints out a BitVector.
// Debug flag must be set to see anything.
//
// v:  The BitVector to print.
//
void bv_print(BitVector *v);

