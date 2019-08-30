#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <sys/sysctl.h>

int main(int argc, char **argv) {
   char *file = argv[1];

   struct stat *buf;
   buf = malloc(sizeof(struct stat));
   //get file stats
   stat(file, buf);
   mode_t mode = buf->st_mode;

      printf("***********************************************\n");
      printf("***Debug output for file stat initialization***\n");
      printf("***********************************************\n");
      char *buf2;
      buf2 = malloc(sizeof(mode_t));
      strmode(mode, buf2);
      printf("file = %s\n", file);
      printf("mode = %s\n", buf2);
      free(buf2);

   int fd;
   fd = open(file, O_RDWR); //read + write mode; file must exist
   if(fd == -1) {
      printf("Error: could not open \"%s\"\n", file);
      return -1;
   }

   //reading from the file
   uint8_t *data;
   data = (uint8_t *) malloc(100 * sizeof(uint8_t));
   int count = read(fd, data, 16);
   printf("Attempted to read 16 bytes from %s\n", file);
   printf("Successfully read %d bytes\n", count);
   printf("Printing contents...\n");
   int i;
   for(i = 0; i < count; i++) {
      printf("%c", data[i]);
   }
   printf("\nDone printing contents.\n");
   close(fd);
   //done reading

   //writing to the output file
   char *output = "output.txt";
   fd = open(output, O_WRONLY);
   printf("Attempting to write to %s\n", output);
   char *string = "I did a write!";
   int writecount = write(fd, string, 16);
   printf("Attempted to write 16 bytes to %s\n", output);
   printf("Successfully write %d bytes\n", writecount);
   
   stat(output, buf);
   mode = buf->st_mode;
      printf("***********************************************\n");
      printf("***Debug output for file stat initialization***\n");
      printf("***********************************************\n");
      char *buf3;
      buf3 = malloc(sizeof(mode_t));
      strmode(mode, buf3);
      printf("file = %s\n", output);
      printf("mode = %s\n", buf3);
      free(buf3);
   close(fd);
   free(buf);
   free(data);
   return 0;
}
