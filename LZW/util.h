// util.h - Contains function declarations of the Utilities module.

#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdarg.h>
#include <stdint.h>

//
// Prints out custom error message and changes exit status.
//
// format:     The string to print when reporting an error.
// ...:        Variable arguments for the string's format.
//
void errprint(const char *format, ...);

//
// Calculates the log base 2 of a 64-bit integer.
//
// n:  The integer to find the log base 2 of.
//
uint8_t log2_64(uint64_t n);

//
// Implementation of the power function.
//
// base:   The base to exponentiate.
// exp:    The power to exponentiate the base to.
//
uint64_t power(uint64_t base, uint64_t exp);

//
// Creates and returns a file descriptor to store contents of stdin.
// A wrapper for the mkstemp() function.
//
int mkstemp_stdin(void);

#endif
