// ll.c - Contains the implementation of the LinkedList ADT.

#include "ll.h"
#include "bv.h"
#include "util.h"
#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// Creates a new ListNode.
//
// @code_num:   The ListNode's code number.
// @word:       The ListNode's word.
// @len:        The length of the ListNode's word.
//
ListNode *ll_node_create(uint64_t code_num, uint8_t *word, uint64_t len) {
  ListNode *n = malloc(sizeof(ListNode));
  assert(n);

  n->wordlen = len;
  n->word = malloc(n->wordlen * sizeof(uint8_t));
  assert(n->word);
  memcpy(n->word, word, len);

  n->code_num = code_num;
  n->next = NULL;

  return n;
}

//
// Frees allocated memory for a ListNode.
//
// @n:  The ListNode to free.
//
void ll_node_delete(ListNode *n) {
  free(n->word);
  n->word = NULL;
  free(n);
  n = NULL;
}

//
// Frees allocated memory for a linked list of ListNodes.
//
// @n:  The head ListNode of the linked list to free.
//
void ll_delete(ListNode *n) {
  while (n) {
    ListNode *temp = n;
    n = n->next;
    ll_node_delete(temp);
  }
}

//
// Searches for a ListNode with the code number in a linked list.
// Returns NULL if the ListNode isn't found.
//
// @n:          The head ListNode of the linked list to search in.
// @code_num:   The code number to search for.
//
ListNode *ll_lookup(ListNode **n, uint64_t code_num) {
  ListNode *prev = NULL;

  for (ListNode *curr = *n; curr; curr = curr->next) {
    if (curr->code_num == code_num) {
      if (prev) {
        prev->next = curr->next;
        curr->next = *n;
        *n = curr;
      }

      return curr;
    }

    prev = curr;
  }

  return NULL;
}

//
// Adds a ListNode into a linked list if it doesn't already exist.
//
// @n:          The head ListNode of the linked list to add to.
// @code_num:   The code number for the ListNode to be inserted.
// @word:       The word for the ListNode to be inserted.
// @len:        The length of the word for the ListNode to be inserted.
//
ListNode *ll_add(ListNode **n, uint64_t code_num, uint8_t *word, uint64_t len) {
  if (ll_lookup(n, code_num)) {
    return *n;
  }

  ListNode *new_head = ll_node_create(code_num, word, len);
  new_head->next = *n;
  return new_head;
}

//
// Prints a ListNode.
// Debug flag must be set to see anything.
//
// @n:  The ListNode to print.
//
void ll_node_print(ListNode *n) {
  printf("\"");

  for (uint64_t i = 0; i < n->wordlen; ++i) {
    printf("%c", (char)n->word[i]);
  }

  printf("\": %ld\n", n->code_num);
}

//
// Prints a linked list of ListNodes.
// Debug flag must be set to see anything.
//
// @n:  The head ListNode of the linked list to print.
//
void ll_print(ListNode *n) {
  for (ListNode *curr = n; curr; curr = curr->next) {
    ll_node_print(curr);
  }
}
