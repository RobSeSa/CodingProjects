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

uint8_t next_char(int infile) {
   if(in_index % (4096 * 8) == 0) {
      int count = read(infile, in_buffer, 4096);
      if( count < 4096) {
         printf("Done reading. %d bytes in buffer left to flush\n", count);
      }
   }
   int byte_index = in_index / 8;
   in_index += 8;
   return in_buffer[byte_index];
}
/*
void buffer_code(int outfile, BitVector *code);
void flush_code(int outfile);
BitVector *next_code(int infile, uint64_t bit_len);
void buffer_word(int outfile, uint8_t *word, uint64_t wordlen);
void flush_word(int outfile);
void print_buffer(uint8_t *b, uint64_t len, int bits);
 */
