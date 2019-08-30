// util.c - Contains the implementation of the Utilities module.

#include <assert.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "util.h"

#define BLOCK_SIZE 4096

int exit_status = EXIT_SUCCESS;

//
// Modification of printf.
// Prints out custom error message and changes exit status.
//
// @format:     The string to print when reporting an error.
// @...:        Variable arguments for the string's format.
//
void errprint(const char *format, ...) {
  va_list arg;
  va_start(arg, format);
  vfprintf(stdout, format, arg);
  va_end(arg);
  exit_status = EXIT_FAILURE;
}

//
// Calculates the log base 2 of a 64-bit integer.
//
// @n:  The integer to find the log base 2 of.
//
uint8_t log2_64(uint64_t n) {
  assert(n != 0);

  uint8_t ret = 0;

  while (n > 1) {
    ret += 1;
    n >>= 1;
  }

  return ret;
}

//
// Implementation of the power function.
//
// @base:   The base to exponentiate.
// @exp:    The power to exponentiate the base to.
//
uint64_t power(uint64_t base, uint64_t exp) {
  uint64_t ret = 1;

  while (exp) {
    if (exp & 1) {
      ret *= base;
    }

    exp >>= 1;
    base *= base;
  }

  return ret;
}

//
// Creates and returns a file descriptor to store contents of stdin.
// A wrapper for the mkstemp() function.
//
int mkstemp_stdin(void) {
  // Make the temporary file and its descriptor.
  char temp[] = "/tmp/lzwcoderXXXXXX";
  int tempfile = mkstemp(temp);

  char buffer[BLOCK_SIZE] = {0};
  int bytes = 0;

  // Write buffered standard input to temporary file.
  while ((bytes = read(STDIN_FILENO, buffer, BLOCK_SIZE)) > 0) {
    write(tempfile, buffer, bytes);
  }

  // Unlink file.
  unlink(temp);

  // Seek to beginning of file descriptor.
  lseek(tempfile, 0, SEEK_SET);

  return tempfile;
}
