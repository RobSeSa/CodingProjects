// ll.h - Contains the function declarations for the LinkedList ADT.

#ifndef __LL_H__
#define __LL_H__

#include <inttypes.h>
#include <stdbool.h>

typedef struct ListNode ListNode;

//
// Struct definition for a ListNode.
//
// next:       The next ListNode in a linked list.
// code_num:   The ListNode's code number.
// word:       The ListNode's word.
// wordlen:    The length of the ListNode's word.
//
struct ListNode {
  ListNode *next;
  uint64_t code_num;
  uint8_t *word;
  uint64_t wordlen;
};

//
// Creates a new ListNode.
//
// code_num:   The ListNode's code number.
// word:       The ListNode's word.
// len:        The length of the ListNode's word.
//
ListNode *ll_node_create(uint64_t code_num, uint8_t *word, uint64_t len);

//
// Frees allocated memory for a ListNode.
//
// n:  The ListNode to free.
//
void ll_node_delete(ListNode *n);

//
// Frees allocated memory for a linked list of ListNodes.
//
// n:  The head ListNode of the linked list to free.
//
void ll_delete(ListNode *n);

//
// Searches for a ListNode with the code number in a linked list.
// Returns NULL if the ListNode isn't found.
//
// n:          The head ListNode of the linked list to search in.
// code_num:   The code number to search for.
//
ListNode *ll_lookup(ListNode **n, uint64_t code_num);

//
// Adds a ListNode into a linked list if it doesn't already exist.
//
// n:          The head ListNode of the linked list to add to.
// code_num:   The code number for the ListNode to be inserted.
// word:       The word for the ListNode to be inserted.
// len:        The length of the word for the ListNode to be inserted.
//
ListNode *ll_add(ListNode **n, uint64_t code_num, uint8_t *word, uint64_t len);

//
// Prints a ListNode.
// Debug flag must be set to see anything.
//
// n:  The ListNode to print.
//
void ll_node_print(ListNode *n);

//
// Prints a linked list of ListNodes.
// Debug flag must be set to see anything.
//
// n:  The head ListNode of the linked list to print.
//
void ll_print(ListNode *n);

# endif
