// protectfile.c
// Robert Sato
// rssato
// CMPS 111 Spring 2019
// Due: Sunday, June 2
// v6.2.19.23:10

// Ray Beaulieu, Stefan Treatman-Clark, Douglas Shors, Bryan Weeks, Jason
// Smith and Louis Wingers. "The SIMON and SPECK lightweight block ciphers," 
// In proceedings of the Design Automation Conference (DAC), 
// 2015 52nd ACM/EDAC/IEEE, pp. 1-6. IEEE, 2015.

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>


#define LCS(X, K)                                                            \
  (X << K) | (X >> (sizeof(uint64_t) * 8 - K)) // left circular shift
#define RCS(X, K)                                                            \
  (X >> K) | (X << (sizeof(uint64_t) * 8 - K)) // right circular shift

// Core SPECK operation
#define R(x, y, k) (x = RCS(x, 8), x += y, x ^= k, y = LCS(y, 3), y ^= x)

void Speck128ExpandKeyAndEncrypt(uint64_t pt[], uint64_t ct[], uint64_t K[]) {
  uint64_t B = K[1], A = K[0];
  ct[0] = pt[0];
  ct[1] = pt[1];
  for (size_t i = 0; i < 32; i += 1) {
    R(ct[1], ct[0], A);
    R(B, A, i);
  }
  return;
}

//checks the char representation of the bytes stored in the 16 byte blocks
//purely used for debug
void checkValue(uint64_t data[]) {
   uint64_t number = data[0];
   uint8_t *p = (uint8_t *)&number;
   int i;
   for(i = 0; i < 8; i++) {
       printf("p[%d]=%c; ", i, p[i]);
   }
   printf("\n");
   number = data[1];
   p = (uint8_t *)&number;
   for(i = 0; i < 8; i++) {
       printf("p[%d]=%c; ", i, p[i]);
   }
   printf("\n");
   return;
}


int main(int argc, char **argv)
{
   if(argc < 4) {
      printf("Error: Missing input arguments\n");
      printf("Example format: ./protectfile -e deadd00d myfile.txt\n");
      printf("add -d to the end for debug mode\n");
      return -1;
   }
   int debug = 0;
   if(argc > 4) {
      char *temp = argv[4];
      if(temp[1] =='d') {
         debug = 1;
      }
   }
   //initializing arguments
   char *option = argv[1];
   if((option[1] != 'e') && (option[1] != 'd')) {
      printf("Error: Invalid option (-e OR -d)\n"); return -1; 
   }
   char *inputkey = argv[2];
   char *file = argv[3];
   struct stat *buf;
   buf = malloc(sizeof(struct stat));
   //get file stats
   stat(file, buf);
   uint64_t inode = buf->st_ino;
   mode_t mode = buf->st_mode;
   //check sticky bit
   if((mode & S_ISTXT) == 0) { //sticky bit not set
      if(option[1] == 'd') {
         printf("No need for decryption.\nExiting...\n");
         return(0);
      }
   }
   else { //sticky bit set
      if(option[1] == 'e') {
         printf("No need for encryption.\nExiting...\n");
         return(0);
      }
   }

   //automatically sets the rights to -rw-r--r--
   char unsticky[] = "0644";
   // 0 = sticky bit
   // 1 = user (owner)
   // 2 = group 
   // 3 = other 
   int j;
   j = strtol(unsticky, 0, 8);
   if (chmod (file, j) < 0) { //turns sticky bit off
       fprintf(stderr, "%s: error in chmod(%s, %s) - %d (%s)\n",
               argv[0], file, unsticky, errno, strerror(errno));
       exit(1); }
   //initializing values
   int i;
   uint64_t *ctr_inode, *value, *key, *data;
   char *eptr;
   value     = (uint64_t *) malloc(2 * sizeof(uint64_t));
   ctr_inode = (uint64_t *) malloc(2 * sizeof(uint64_t));
   key       = (uint64_t *) malloc(2 * sizeof(uint64_t));
   data      = (uint64_t *) malloc(2 * sizeof(uint64_t));
   ctr_inode[0] = inode; // file ID (i-number)
   ctr_inode[1] = 0; // CTR value(byte offset in 16 byte chunks)
   key[0] = 0; // high order 64 bits of the key
   key[1] = strtoll(inputkey, &eptr, 16); // low order 64 bits of the key

   //file handling
   int fd;
   fd = open(file, O_RDWR); //read + write mode; file must exist
   if(fd == -1) {
      printf("Error: could not open \"%s\"\n", file); 
      printf("Error Number % d\n", errno);  
      return -1;
   }

   // *******************************************************
   // *****************Encryption/Decryption*****************
   // *******************************************************
   uint64_t count = 0, total = 0; //count of how many bytes read
   int writecount = 0;
   int seek_cur;
   while((count = read(fd, &data[0], 8)) > 0) {
      if(count == 8) {//keep reading
         count += read(fd, &data[1], 8);
      }
      else { data[1] = 0; }
      //get a new value for each 16 byte offset
      ctr_inode[1] = total/16; //16 byte offset
      Speck128ExpandKeyAndEncrypt(ctr_inode, value, key);
      if(debug == 1) {//if in debug mode print contents as read in
           printf("Printing Speck parameters\n");
   printf("ctr_inode[0] = %llu\n", ctr_inode[0]);
   printf("ctr_inode[1] = %llu\n", ctr_inode[1]);
   printf("key[0] = %llX\n", key[0]);
   printf("key[1] = %llX\n", key[1]);
   printf("value[0] = %llX\n", value[0]);
   printf("value[1] = %llX\n", value[1]);
           uint64_t tempval = value[0] << 4;
           printf("value[0] << 4 = %llX\n", tempval);

         printf("Count = %lu\n", count);
         printf("Data before encryption:\n");
         checkValue(data); }
      // XOR the 16 bytes read with the encryption value
      data[0] ^= value[0];
      data[1] ^= value[1];
      total += count;
      seek_cur = lseek(fd, (0-count), SEEK_CUR);
      if(debug == 1) {//if in debug mode print contents after encrypted 
         printf("***data ^= value***\n");
         printf("seek_cur = %d\n", seek_cur);
         printf("Data after encryption:\n");
         checkValue(data);
         printf("\n"); }
      if(count < 9) { //write to first half of data
         writecount = write(fd, &data[0], count);
         if(writecount != count) {
            printf("Error: Tried writing %lu bytes.\n", count); 
            printf("Successfully wrote %d bytes.\n", writecount );
         }
      }
      else { //write to both halves of data
         writecount = write(fd, &data[0], 8);
         writecount += write(fd, &data[1], (count-8));
         if(writecount != count) {
            printf("Error: Tried writing %lu bytes.\n", count); 
            printf("Successfully wrote %d bytes.\n", writecount );
         }
      }
   }
   close(fd);
   
   //turn sticky bit back on if just encrypted
   //leave off if decrypt
   if(option[1] == 'e') {
      char resticky[] = "1644";
      j = strtol(resticky, 0, 8);
      if (chmod (file, j) < 0) { //turns sticky bit on 
          fprintf(stderr, "%s: error in chmod(%s, %s) - %d (%s)\n",
                  argv[0], file, resticky, errno, strerror(errno));
          exit(1); }
   }
   if(debug == 1){
      printf("total bytes read = %lu\n", total); }

   free(buf);
   free(ctr_inode);
   free(value);
   free(key);
   free(data);
   printf("Exiting...\n");
   return(0);
}
/* prints mode of a file
   if(debug == 1){
      printf("***********************************************\n");
      printf("***Debug output for file stat initialization***\n");
      printf("***********************************************\n");
      char *buf2;
      buf2 = malloc(sizeof(mode_t));
      strmode(mode, buf2);
      printf("option = %c\n", option[1]);
      printf("inputkey = %s\n", inputkey);
      printf("file = %s\n", file);
      printf("inode = %llu\n\n", inode);
      printf("mode = %s\n", buf2);
      free(buf2); }

*/
