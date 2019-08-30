//myshell.c
//Robert Sato
//rssato
//CMPS 111 Spring 2019
//Due: Sunday, April 21

#include <stdio.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>//for catching ^c

#define SIZE 100

extern char **getLine();

//handles the ctrl c signal
pid_t child_pid = -1;
static void sig_handler(int _) {
   (void)_;
   if(child_pid != -1){
      kill(child_pid, SIGTERM);
   }
}

//helper function for debugging that prints contents of an array
void printStringArray(char *name, char **args, int start, int end) {
   int i;
   printf("***Printing string array for: %s\n", name);
   for(i = start; i < end; i++){
      printf("%s[%d] = %s\n", name, i, args[i]);
   }
}

//helper function for debugging that prints contents of an array
void printArray(char *name, int args[], int start, int end) {
   int i;
   printf("***Printing int array for: %s\n", name);
   for(i = start; i < end; i++){
      printf("%s[%d] = %d\n", name, i, args[i]);
   }
}

//helper function for closing pipes
void closePipes(int pipes[], int count) {
   int i;
   for(i = 0; i < count*2; i++) {
      close(pipes[i]);
   }
}

//executes given command with proper format of args array and files
int execute(char **args, char *inputFile, char *outputFile, int append, int waitflag) {
   int fdin, fdout;
   pid_t pid = fork();
   if(pid < 0) { //error occurred
      printf("Error has occurred in forking child process\n");
      printf("pid = %d\n", pid);
      perror("fork");
      exit(1);
   }
   else if(pid == 0) { //child process
      if(inputFile == NULL) {//no input file but given <
         //printf("**No input file detected\n");
      }
      else {//input file detected
         fdin = open(inputFile, O_RDONLY);
         if(fdin < 0) {//error occurred opening file
            printf("**Error occurrred opening file %s\n", outputFile);
            char cwd[1024];
            getcwd(cwd, sizeof(cwd));
            printf("The file %s/%s does not exist.\n", cwd, inputFile);
            return -1;
         }
         else {//file opened properly
            close(0);
            dup(fdin);
            close(fdin);
         }
      }
      if(outputFile == NULL) {//no output file but given > or >>
         //printf("**No output file detected\n");
      }
      //append to output file mode
      else if (append == 1) {
         fdout = open(outputFile, O_CREAT | O_WRONLY | O_APPEND, 0666/*S_IWUSR*/);
         if(fdout < 0) {//error occurred opening file
            printf("**Error occurrred opening file %s\n", outputFile);
            return -1;
         }
         else {//file opened properly
            close(1);
            dup(fdout);
            close(fdout);
         }
      }
      //overwrite output file mode
      else {
         fdout = open(outputFile, O_CREAT | O_WRONLY, 0666);
         if(fdout < 0) {//error occurred opening file
            printf("**Error occurrred opening file %s\n", outputFile);
            return -1;
         }
         else {//file opened properly
            close(1);
            dup(fdout);
            close(fdout);
         }
      }
      //execute the command and check if fail
      if(execvp(args[0], args) < 0) { 
         printf("Error has occurred executing the command: %s\n", args[0]);
         perror("command");
         exit(1);
      }
      exit(0);
   }
   else { //parent process
      child_pid = pid;
      if(waitflag == 1) {
         waitpid(pid, (void *) 0, 0);
      }
   }
   return 0;
}

//executes piped commands
int executePipes(char **args, char *inputFile, char *outputFile, 
                 int pipeCount, int *commands, int append, int waitflag) {
   int i, status, cmdCounter = 0, commandCount = pipeCount + 1;
   int fdin, fdout;
   int pipes[pipeCount*2];
   for(i = 0; i < pipeCount; i++) {
      pipe(pipes + (i*2));
   }
   if(fork() == 0) {//child process executing first command
      if(inputFile == NULL) {//no input file 
         //printf("**No input file detected\n");
      }
      else {//input file detected
         fdin = open(inputFile, O_RDONLY);
         if(fdin < 0) {//error occurred opening file
            printf("**Error occurrred opening file %s\n", outputFile);
            return -1;
         }
         else {//file opened properly
            close(0);
            dup(fdin);
            close(fdin);
         }
      }
      close(1);
      dup(pipes[1]);
      closePipes(pipes, pipeCount);
      //execute the command at proper index
      if(execvp(args[(commands[cmdCounter])], &args[(commands[cmdCounter])]) < 0) {
         printf("Error has occurred executing the command: %s\n", 
                 args[(commands[cmdCounter])]);
         perror("command");
         exit(1);
      }
   }
   else { //parent process
      //executes inner commands that have pipe input and output
      cmdCounter++;
      if(pipeCount > 1) {
         for(i = 0; i < (pipeCount*2)-2; i += 2) {
            printf("i = %d\n", i);
            if(fork() == 0) {
               close(0); dup(pipes[i]);
               close(1); dup(pipes[i+3]);
               closePipes(pipes, pipeCount);
               if(execvp(args[(commands[cmdCounter])],
                         &args[(commands[cmdCounter])]) < 0) {
                  perror("command");
                  exit(1);
               }
            }
            else { cmdCounter++; }
         }
      }//end if pipeCount > 1
      if(fork() == 0) {//executes the last command in pipeline
         if(outputFile == NULL) {//no output file
            //printf("**No output file detected\n");
         }
         //append to output file mode
         else if (append == 1) {
            fdout = open(outputFile, O_CREAT | O_WRONLY | O_APPEND, 0666/*S_IWUSR*/);
            if(fdout < 0) {//error occurred opening file
               printf("**Error occurrred opening file %s\n", outputFile);
               return -1;
            }
            else {//file opened properly
               close(1);
               dup(fdout);
               close(fdout);
            }
         }
         //overwrite output file mode
         else {
            fdout = open(outputFile, O_CREAT | O_WRONLY, 0666);
            if(fdout < 0) {//error occurred opening file
               printf("**Error occurrred opening file %s\n", outputFile);
               return -1;
            }
            else {//file opened properly
               close(1);
               dup(fdout);
               close(fdout);
            }
         }
         close(0);
         if(pipeCount > 0) {
            dup(pipes[(pipeCount*2)-2]);
         }
         else {
            dup(pipes[0]);
         }
         closePipes(pipes, pipeCount);
         //execute the command at proper index
         if(execvp(args[(commands[cmdCounter])], &args[(commands[cmdCounter])]) < 0) {
            printf("Error has occurred executing the command: %s\n", 
                    args[(commands[cmdCounter])]);
            perror("command");
            exit(1);
         }
      }
   }
   closePipes(pipes, pipeCount);
   //wait for all children to finish
   if(waitflag == 1) {
      for(i = 0; i < commandCount; i++) {
         wait(&status);
      } 
   }
   return 0;
}

//handles converting the input array of strings into a format useable by execute function
void handler(char **args, int start, int end) {
   int i, counter = 0;
   int pipeCommands[SIZE];
   pipeCommands[0] = 0;
   char *temp[SIZE];
   temp[0] = NULL;
   char *input = NULL;
   char *output = NULL;
   int append = 0, pipeCount = 0, waitflag = 1;
   if(strcmp(args[start], "exit") == 0) {
      for(i = start; i < end; i++) {
         free(args[i]);
      }
      printf("Goodbye...\n");
      exit(0);
   }
   for(i = start; i < end; i++) {
      //input redirection case
      if(strcmp(args[i], "<") == 0) {
         input = args[++i];
         if(input == NULL) {
            printf("syntax error no input file specified\n");
         }
      }
      //output redirection case
      else if(strcmp(args[i], ">") == 0) {
         output= args[++i];
         if(output == NULL) {
            printf("syntax error no output file specified\n");
         }
      }
      //append output redirection case
      else if(strcmp(args[i], ">>") == 0) {
         output= args[++i];
         if(output == NULL) {
            printf("syntax error no output file specified\n");
         }
         else {
            append = 1;
         }
      }
      //pipe case
      else if(strcmp(args[i], "|") == 0) {
         pipeCount++;
         temp[counter] = NULL;
         temp[++counter] = NULL;
         pipeCommands[pipeCount] = i+1;
      }
      //multiple command case
      else if(strcmp(args[i], ";") == 0) {
         //breaks the loop and goes directly to execution
         break;
      }
      //do not call wait case
      else if(strcmp(args[i], "&") == 0) {
         waitflag = 0;
      }
      //( ) not handled case
      else if( (strcmp(args[i], "(") == 0) || (strcmp(args[i], ")") == 0)) {
         printf("( or ) not supported\n");
      }
      //no special characters
      //execute single command with/without args normally
      else {
         temp[counter] = args[i];
         temp[++counter] = NULL;
      }
   }//end for loop
   //pipe(s) detected
   if(pipeCount > 0) {
      executePipes(temp, input, output, pipeCount, pipeCommands, append, waitflag);
   }
   //no pipes detected
   else {
      if((execute(temp, input, output, append, waitflag)) < 0){
         printf("***Error occurred in execute()\n");
      }
   }
   //more commands to execute
   if(i < end) {
      handler(args, i+1, end);
   }
}


int main() {
   char **args;
   int i, size = 0;
   printf("Welcome to Robert Sato's Shell!\n");
   printf("Enter commands at the $ prompt.\n");
   signal(SIGINT, sig_handler);
   while(1) {
      //system prompt
      printf("$ ");
      args = getLine();
      for(i = 0; args[i] != NULL; i++) {
         size++;
      }
      if(size > 0) {
         handler(args, 0, size);
         size = 0;
      }
   } 
   return 0;
}
