#include "bv.h"
#include "io.h"
#include <inttypes.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>

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
   printf("read_header called!\n");
   int bytes_read;
   bytes_read = read(infile, &(header->magic), 4);
   if(bytes_read != 4) { 
      printf("error reading magic number\n");
      //return;
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
   printf("magic = %u, file_size = %lu, protection = %u\n", 
          header->magic, header->file_size, header->protection);
}
void write_header(int outfile, FileHeader *header) {
   printf("write_header called!\n");
   int bytes_written;
   bytes_written = write(outfile, &(header->magic), 4);
   bytes_written += write(outfile, &(header->file_size), 8);
   bytes_written += write(outfile, &(header->protection), 2);
   bytes_written += write(outfile, &(header->padding), 2);
   if(bytes_written != 16) { printf("Error: Did not write 16 bytes :o\n"); }
   printf("Successfully wrote magic = %u, file_size = %lu, protection = %u\n", 
          header->magic, header->file_size, header->protection);
}
/*
uint8_t next_char(int infile);
void buffer_code(int outfile, BitVector *code);
void flush_code(int outfile);
BitVector *next_code(int infile, uint64_t bit_len);
void buffer_word(int outfile, uint8_t *word, uint64_t wordlen);
void flush_word(int outfile);
void print_buffer(uint8_t *b, uint64_t len, int bits);
 */
