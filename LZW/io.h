// io.h - Contains the function declarations for the I/O module.

#ifndef __IO_H__
#define __IO_H__

#include <inttypes.h>
#include "bv.h"

#define MAGIC           0xdeadcafe
#define BLOCK_SIZE      4096
#define BITS_IN_BYTE    8



//
// Struct definition for a compressed file's header.
//
// magic:       Magic number that verifies this compression program.
// file_size:   File size of original, uncompressed file.
// protetction: The original file's permission bits.
// padding:     Pads struct out to 32 bits.
//
typedef struct FileHeader {
  uint32_t magic;
  uint64_t file_size;
  uint16_t protection;
  uint16_t padding;
} FileHeader;

//
// Reads a file header from the input file.
//
// infile:     The input file descriptor.
// header:     The file header struct to read into.
//
void read_header(int infile, FileHeader *header);

//
// Writes a file header to the output file and returns original file size.
//
// outfile:    The output file descriptor.
// header:     FileHeader to set values in and write.
//
void write_header(int outfile, FileHeader *header);

//
// Returns next character, or byte, from the input file.
// 4KB of characters are read when needed into a character buffer.
// This is only called once for each byte in the input file.
//
// infile:     The input file descriptor.
//
uint8_t next_char(int infile);

//
// Adds code into a code buffer to write to the output file.
// Writes when 4KB of codes have been buffered.
//
// outfile:    The output file descriptor.
// v:          BitVector code to add to code buffer.
//
void buffer_code(int outfile, BitVector *code);

//
// Flushes any remaining codes in the buffer to the output file.
//
// outfile:    The output file descriptor.
//
void flush_code(int outfile);

//
// Reads and returns next code in the input file.
// 4KB worth of codes are read into a code buffer.
// Called until main decompression loop decodes all characters.
//
// infile:     The input file descriptor.
// bit_len:    The length in bits of the code to read.
//
BitVector *next_code(int infile, uint64_t bit_len);

//
// Adds a word to the character buffer to write to the output file.
// Buffer is written when it's filled with 4KB of characters.
//
// outfile:    Output file to write characters to.
// word:       Word to output.
// word:       Length of word to output.
//
void buffer_word(int outfile, uint8_t *word, uint64_t wordlen);

//
// Flushes the character buffer to the output file if not empty.
//
// outfile:    Output file to flush characters to.
//
void flush_word(int outfile);

//
// Prints out a buffer.
// Debug flag must be set to see anything.
//
// b:      The buffer to print.
// len:    The size of the buffer.
// bits:   Flag for whether to print bits or bytes.
//
void print_buffer(uint8_t *b, uint64_t len, int bits);

#endif
