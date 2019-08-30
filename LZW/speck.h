#ifndef __SPECK_H__
#define __SPECK_H__

#include <inttypes.h>

//
// Returns the hash value for a string, using a salt as a key.
//
// s:          The string to compute keyed hash for.
// length:     The length of the string.
// key:        The key or salt for the keyed hash.
//
uint64_t keyed_hash(const char *s, uint32_t length, uint64_t *key);

#endif
