// main.c

#include "ll.h"
#include "bv.h"
#include "trie.h"
#include "io.h"
#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv) {
   printf("\nThis is the main function of LZW\n");
   int vflag = 0, cflag = 0, dflag = 0, iflag = 0, oflag = 0;
   char *inputFile = NULL;
   int inputFd = 0, outputFd = 1;
   char *outputFile = NULL;
   int c;

   struct stat fileStat;
   fileStat.st_mode = 0;
   while((c = getopt (argc, argv, "vcdi:o:")) != -1) {
      switch(c) {
         case 'v':
            vflag = 1;
            break;
         case 'c':
            cflag = 1;
            break;
         case 'd':
            dflag = 1;
            break;
         case 'i':
            iflag = 1;
            inputFile = optarg;
            inputFd = open(inputFile, O_RDONLY);
            //get stats for input file
            if(stat(inputFile, &fileStat) < 0) { return 1; }
            break;
         case 'o':
            oflag = 1;
            outputFile = optarg;
            if(inputFd != 0) {
               outputFd = open(outputFile, O_WRONLY | O_CREAT, fileStat.st_mode);
               if(outputFd == -1) { printf("error opening %s\n", outputFile); }
            }
            else {
               outputFd = open(outputFile, O_WRONLY | O_CREAT);
               if(outputFd == -1) { printf("error opening %s\n", outputFile); }
            }
            break;
/*
         case 'x':
            outputFile = optarg;
            if(inputFd != 0) {
               outputFd = open(outputFile, O_WRONLY | O_CREAT, fileStat.st_mode);
               if(outputFd == -1) { printf("error opening %s\n", outputFile); }
            }
            else {
               outputFd = open(outputFile, O_WRONLY | O_CREAT);
               if(outputFd == -1) { printf("error opening %s\n", outputFile); }
            }
            FileHeader *fh = file_header_create(MAGIC, fileStat.st_size, 
                                                fileStat.st_mode, 0x0000);
            printf("Writing header from %s to %s\n", outputFile, inputFile);
            write_header(outputFd, fh);
            return 0;
*/
         default:
            printf("Usage: ./lzwcoder -vcdi:o:\n");
            break;
      }
   }
   if(vflag) {
      printf("vflag = %d, cflag = %d, dflag = %d, iflag = %d, oflag = %d\n",
             vflag, cflag, dflag, iflag, oflag);
      printf("inputfile = %s, outputFile = %s\n", inputFile, outputFile);
      printf("%s file size = %lu\n", inputFile, fileStat.st_size);
      if(iflag) {
         printf("File Permissions for %s:", inputFile);
         printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
         printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
         printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
         printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
         printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
         printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
         printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
         printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
         printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
         printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
         printf("\n");
      }
   }
   FileHeader *fh = file_header_create(MAGIC, fileStat.st_size, fileStat.st_mode, 0x0000);
   write_header(outputFd, fh);
   read_header(inputFd, fh);
/*
   FileHeader *fh = file_header_create(MAGIC, 16, 33188, 0x0000);
   write_header(outputFd, fh);
   */
   free(fh);


   if(iflag) { close(inputFd); }
   if(oflag) { close(outputFd); }
   return 0;
}


   //ListNode testing
/*   char *word = "temp";
   ListNode *listNode = ll_node_create(98, (uint8_t *)word, 4);
   printf("Printing the contents of listNode\n");
   ll_node_print(listNode);
   ll_node_delete(listNode);
*/
   //BitVector testing
/*   printf("Creating BitVector\n");
   BitVector *bv = bv_create(10);
   uint64_t byte_length = byte_len(10);
   printf("bv's length = %ld\n", byte_length);
   printf("getbit 9\n");
   printf("%u\n",  bv_get_bit(bv, 9) );
   bv_set_bit(bv, 9);
   printf("getbit 9 after setting\n");
   printf("%u\n",  bv_get_bit(bv, 9) );
   bv_clr_bit(bv, 9);
   printf("getbit 9 after clearing\n");
   printf("%u\n",  bv_get_bit(bv, 9) );
   printf("Deleting the bv\n");
   bv_delete(bv);
*/
   //More BitVector testing
/*   BitVector *codebv = code_num_to_bv(5, 9);
   printf("contents of codebv(printed from LSB in bv->vector): ");
   bv_print(codebv);
   printf("\n");
   BitVector *temp = codebv;
   uint64_t code = 0;
   //infer says codebv unreachable after this point
   code = bv_to_code_num(codebv);
   printf("codebv->length = %u\n", codebv->length);
   printf("code = %lu\n", code);
   bv_delete(temp);
*/
   //TrieNode testing
/*   TrieNode *tn = trie_create();
   TrieNode *root = tn;
   printf("tn->sym = %u\ntn->code_num = %lu\n", tn->sym, tn->code_num);
   trie_print(tn, 0);
   printf("\nStepping through the tree\n");
   char *word = "word";
   int i = 0;
   while(tn != NULL) {
      trie_node_print(tn);
      printf("\n");
      printf("word[i] = %u\n", (uint8_t)word[i]);
      tn = trie_step(tn, (uint8_t)word[i]);
      i++;
   }
   trie_delete(root);
*/
/*
   //hashTable.h testing
   HashTable *ht = ht_create(500);
   ht_print(ht);
   uint64_t code_num = 256;
   //printf("hash(ht, 256) = %lu\n", hash(ht, 256));
   if(ht_lookup(ht, code_num) != NULL) {
      printf("found!\n");
   }
   else { printf("not found :/\n"); }
   uint8_t *word = (uint8_t*)"word";
   printf("adding %lu, %s...\n", code_num, word);
   ht_add(ht, code_num, word, 4);
   if(ht_lookup(ht, code_num) != NULL) {
      printf("found!\n");
   }
   else { printf("not found :/\n"); }
   ht_delete(ht);
*/
