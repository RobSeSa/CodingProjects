#include "bv.h"
#include "io.h"
#include "endian.h"
#include <inttypes.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

FileHeader *file_header_create(uint32_t magic, uint64_t file_size,
                               uint16_t protection, uint16_t padding) {
   FileHeader *fh = malloc(sizeof(FileHeader));
   fh->magic = magic;
   fh->file_size = file_size;
   fh->protection = protection;
   fh->padding = padding;
   return fh;
}

void read_header(int infile, FileHeader *header) {
   printf("**read_header called!\n");
   int bytes_read;
   bytes_read = read(infile, &(header->magic), 4);
   if(bytes_read != 4) { 
      printf("error reading magic number\n");
      //return;
   }
   if(header->magic != MAGIC) {
      printf("Incorrect magic number.\nExiting...\n");
      return;
   }
   bytes_read = read(infile, &(header->file_size), 8);
   if(bytes_read != 8) { 
      printf("error reading file size\n"); 
      //return;
   }
   bytes_read = read(infile, &(header->protection), 2);
   if(bytes_read != 2) { 
      printf("error reading protection\n");
      //return;
   }
   bytes_read = read(infile, &(header->padding), 2);
   if(bytes_read != 2) { 
      printf("error reading padding\n");
      //return;
   }
   if(is_big()) { 
      printf("This system is big endian\nSwapping endian\n");
      header->magic = swap32(header->magic);
      header->file_size = swap64(header->file_size);
      header->protection = swap16(header->protection);
      header->padding = swap16(header->padding);
   }
   printf("READ: magic = %u, file_size = %lu, protection = %07o, padding = %u\n", 
          header->magic, header->file_size, header->protection, header->padding);
}
void write_header(int outfile, FileHeader *header) {
   printf("**write_header called!\n");
   uint32_t magic = header->magic;
   uint64_t file_size = header->file_size;
   uint16_t protection = header->protection;
   uint16_t padding = header->padding;
   int bytes_written;
   if(is_big()) { 
      printf("This system is big endian\nSwapping endian\n");
      magic = swap32(magic);
      file_size = swap64(file_size);
      protection = swap16(protection);
      padding = swap16(padding);
   }
   bytes_written = write(outfile, &(magic), 4);
   bytes_written += write(outfile, &(file_size), 8);
   bytes_written += write(outfile, &(protection), 2);
   bytes_written += write(outfile, &(padding), 2);
   if(bytes_written != 16) { printf("Error: Did not write 16 bytes :o\n"); }
   printf("WRITE: magic = %u, file_size = %lu, protection = %07o, padding = %u\n", 
          magic, file_size, protection, padding);
}

uint8_t in_buffer[4096];
uint64_t in_index = 0;//index in bits
uint16_t total = 0;//total bytes in buffer

uint8_t next_char(int infile) {
  if(total == 0) {
      total = read(infile, in_buffer, 4096);
      if(total == 0) {
         printf("Error: Read too many bytes in next_char\n");
         return 0;
      }
   }
   int byte_index = (in_index / 8) % 4096;
   in_index += 8;
   total--;
   return in_buffer[byte_index];
}

uint8_t out_buffer[4096];
uint64_t out_index = 0;

void buffer_code(int outfile, BitVector *code) {
   BitVector *temp = bv_create(8);
   uint64_t i;
   uint8_t j = 0;
   for(i = 0; i < code->length; i++) {
      //printf("i = %lu, j = %u, out_index = %lu\n", i, j, out_index);
      if((out_index != 0) && ((out_index%(4096*8)) == 0)) {//buffer is full
         write(outfile, out_buffer, 4096);
         out_index = 0;
      }
      if(((out_index % 8) == 0) && (out_index != 0)) {//set the byte in out_buffer
         printf("setting out_buffer[%lu] = ", (out_index/8) - 1);
         bv_print(temp);
         printf("\n");
         out_buffer[(out_index/8) - 1] = temp->vector[0];
         j = 0;
      }
      if(bv_get_bit(code, i)) {
         bv_set_bit(temp, j);
      }
      else {
         bv_clr_bit(temp, j);
      }
      j++;
      out_index++;
   }
   if(((out_index % 8) == 0) && (out_index != 0)) {//set the byte in out_buffer
      printf("setting out_buffer[%lu] = ", (out_index/8) - 1);
      bv_print(temp);
      printf("\n");
      out_buffer[(out_index/8) - 1] = temp->vector[0];
   }
}

//needs further testing
void flush_code(int outfile) {
   if(out_index != 0) {
      //flush code
      int total;
      total = write(outfile, out_buffer, (out_index/8));
      if(out_index%8 != 0) {//one more byte to write out
         BitVector *temp = bv_create(8);
         uint64_t i;
         for(i = 0; i < 8; i++) {//iterate through the byte
            if(i < out_index % 8) {//if values exist in the buffer
               if((out_buffer[out_index/8] >> i) & 1u) {//1 in the buffer
                  bv_set_bit(temp, i);
               }
               else {//0 in the buffer
                  bv_clr_bit(temp, i);
               }
            }
            else {//pad with 0s
               bv_clr_bit(temp, i);
            }
         }
         total += write(outfile, &(temp->vector[0]), 1); //write out the one byte
         bv_delete(temp);
      }
      printf("flush_code: Flushed out %d bytes\n", total);
   }
}
/*
uint8_t in_buffer[4096];
uint64_t in_index = 0;//index in bits
uint16_t total = 0;//total bytes in buffer
*/
BitVector *next_code(int infile, uint64_t bit_len) {
   BitVector *temp = bv_create(bit_len);
   uint64_t i;
   in_index = 0;
   total = 0;
   for(i = 0; i < bit_len; i++) {
      printf("i = %lu, in_index = %lu\n", i, in_index);
      if(in_index == total * 8) {//need to read buffer
         total = read(infile, in_buffer, 4096);
         in_index = 0;
         if(total == 0) { 
            printf("next_code: No more bytes to read\n");
            break;
         }
      }
      if((in_buffer[in_index/8] >> (in_index % 8)) & 1u) {//1
         bv_set_bit(temp, i);
      }
      else {//0
         bv_clr_bit(temp, i);
      }
      in_index++;
      if((i%8 == 0) && (i != 0)) {//decrement total per byte
         total--;
      }
   }
   printf("next_code: Finished reading next code:");
   bv_print(temp);
   printf("\n");
   return temp;
}
void buffer_word(int outfile, uint8_t *word, uint64_t wordlen) {
   printf("buffer_word\n");
   uint64_t i;
   out_index = 0;
   for(i = 0; i < wordlen; i++) {
      if(out_index == 4096 * 8) {//buffer full
         write(outfile, out_buffer, 4096);
         print_buffer(out_buffer, 4096, 0);
         out_index = 0;
      }
      out_buffer[out_index/8] = word[i];
      out_index += 8;
   }
}

void flush_word(int outfile) {
   printf("flush_word\n");
   if(out_index != 0) {
      write(outfile, out_buffer, out_index/8);
      print_buffer(out_buffer, out_index/8, 0);
   }
}
void print_buffer(uint8_t *b, uint64_t len, int bits) {
   if(bits) {
      printf("printing buffer as bits\n");
      uint64_t i;
      BitVector *temp = bv_create(8);
      for(i = 0; i < len; i++) {
         temp->vector[0] = b[i];
         bv_print(temp);
      }
      printf("\n");
      bv_delete(temp);
   }
   else {
      printf("printing buffer as words\n");
      uint64_t i;
      for(i = 0; i < len; i++) {
         printf("%c", b[i]);
      }
      printf("\n");
   }
}
