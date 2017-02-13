/*
 *
 * Author: Jason Bensel
 * Version: CIS452 - Project 2 - 2/3/2017
 * Description: CIS452 Project 1, Streamed Vector Processing
 *
 */

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define READ 0
#define WRITE 1

void complimentSigHandler(int);
void incSigHandler(int);
void addSigHandler(int);


int main(int argc, char const *argv[]){
  //File pointer
  FILE *fptr;
  //Mode for fopen
  char mode = 'r';
  //Line buffer from read line
  char *loadBuffer;
  //Output buffer
  int *writeBuffer;
  //Line size binary number
  int lineSize;

  //PID's for processes
  pid_t child_one, child_two, child_three;
  //Create read and write file descriptors
  int pipeMain[2];
  int pipeToComplimenter[2];
  int pipeToIncrementer[2];
  int pipeToAdder[2];


  size_t MAX_LINE_SIZE = 20;
  loadBuffer = malloc(MAX_LINE_SIZE);
  writeBuffer = malloc(MAX_LINE_SIZE);


  //Check for valid arguments
  if(argc != 3){
    perror("You need to enter in desired filenames");
    exit(1);
  }

  //Create pipes with declared file descriptors
  // if(pipe(pipeMain) == -1)
  // {
  //   perror("Pipe failure");
  //   exit(1);
  // }
  if(pipe(pipeToComplimenter) == -1){
    perror("Pipe failure");
    exit(1);
  }
  // if(pipe(pipeToIncrementer) == -1){
  //   perror("Pipe2 failure");
  //   exit(1);
  // }
  // if(pipe(pipeToAdder) == -1){
  //   perror("Pipe2 failure");
  //   exit(1);
  // }

  //Create child fork for complimenter
  if((child_one = fork()) < 0){
    perror("Fork error");
  }
  // //Create child fork for incrementer
  // if((child_two = fork()) < 0){
  //   perror("Fork error 2");
  // }
  // //Create child fork for adder
  // if((child_three = fork() < 0)){
  //   perror("Fork error 2");
  // }



  /** CHILD ONE - COMPLIMENTER **/
  if(child_one == 0){

    //close(pipeToComplimenter[WRITE]);
    close(pipeToIncrementer[READ]);
    //Redirect pipes
    //dup2(pipeToComplimenter[READ], STDIN_FILENO);
    //close(pipeToComplimenter[READ]);
    // close(pipeToComplimenter[READ]);
    // close(pipeToComplimenter[WRITE]);
    // close(pipeMain[READ]);
    // close(pipeMain[WRITE]);
    // dup2(pipeToIncrementer[WRITE], STDOUT_FILENO);
    //
    //Close unused pipes
    // close(pipeMain[READ]);
    // close(pipeMain[WRITE]);
    // close(pipeToComplimenter[READ]);
    // close(pipeToComplimenter[WRITE]);
    //close(pipeToIncrementer[READ]);
    // close(pipeToIncrementer[WRITE]);
    // close(pipeToAdder[READ]);
    // close(pipeToAdder[WRITE]);

    pid_t ppid = getppid();
    char inputBuffer[1];
    while(read(pipeToComplimenter[READ], inputBuffer, 1) > 0){
      printf("%c", inputBuffer[0]);
      //write(pipeToIncrementer[WRITE], &inputBuffer[0], 1);
      kill(ppid, SIGUSR1);
    }
    //
    // //Get parent process ID
    // pid_t ppid = getppid();
    //
    // char inputBuffer[1];
    // while(read(pipeToComplimenter[READ], inputBuffer, 1) > 0){
    //   char c = inputBuffer[0];
    //   if(inputBuffer[0] == '0'){
    //     c = '1';
    //   }else{
    //     c = '0';
    //   }
    //   printf("%c:INCREMENTER: %c\n",c, inputBuffer[0]);
    //   kill(ppid, SIGUSR1);
    // }


    // dup2(pipefd1[READ], STDIN_FILENO); // connect read to stdin
    // //close(pipefd1[WRITE]); //Close the write end of the pipe from PARENT
    // close(pipefd2[READ]); //Close the read end of pipe to child2
    //
    //
    // // close(pipefd[READ]); //close read end;
    // char inputBuffer[1];
    //
    // //Read incoming characters from pipe
    // while(read(pipefd1[READ], inputBuffer, 1) > 0){
    //   char c = inputBuffer[0];
    //   if(inputBuffer[0] == '0'){
    //     c = '1';
    //   }else{
    //     c = '0';
    //   }
    //
    //   printf("%c:INCREMENTER: %c\n",c, inputBuffer[0]);
    //   dup2(pipefd2[WRITE], STDOUT_FILENO);
    //   write(pipefd2[WRITE], &c, 1);
    //   signal(SIGUSR2, addSigHandler);
    //   pause();
    //   kill(ppid, SIGUSR1);
      //write(pipefd2[WRITE], &c, 1);

      //printf("%c:", c);


    //}
  }

  /** CHILD TWO - INCREMENTER **/
  // if(child_two == 0){
  //
  //   //Close unused pipes
  //   // close(pipeMain[READ]);
  //   // close(pipeMain[WRITE]);
  //   close(pipeToComplimenter[READ]);
  //   close(pipeToComplimenter[WRITE]);
  //   // //close(pipeToComplimenter[WRITE]);
  //   //close(pipeToIncrementer[READ]);
  //   close(pipeToIncrementer[WRITE]);
  //   // close(pipeToAdder[READ]);
  //   // close(pipeToAdder[WRITE]);
  //
  //   //Get parent process ID
  //   pid_t ppid = getppid();
  //
  //
  //   char inputBuffer[1];
  //
  //   //Read incoming characters from pipe2
  //   while(read(pipeToIncrementer[READ], inputBuffer, 1) > 0){
  //     printf("%c\n", inputBuffer[0]);
  //     //kill(ppid, SIGUSR2);
  //
  //
  //
  //     //write(pipefd2[WRITE], &c, 1);
  //   }
  //
  // }

  /** CHILD TWO - INCREMENTER **/
  // if(child_three == 0){
  //
  // }

  /** PARENT PROCESS **/
  else
  {
    //Open the file stream
    if((fptr = fopen(argv[1], &mode)) == NULL)
    {
      perror("Error opening file");
      return -1;
    }

    //Redirect pipes
    //pipeToComplimenter[WRITE] = pipeMain[WRITE];
    //pipeToComplimenter[READ] = pipeMain[READ];

    //Close unused pipes
    // close(pipeMain[READ]);
    // close(pipeMain[WRITE]);
    close(pipeToComplimenter[READ]);
    // //close(pipeToComplimenter[WRITE]);
    // close(pipeToIncrementer[READ]);
    // close(pipeToIncrementer[WRITE]);
    // close(pipeToAdder[READ]);
    // close(pipeToAdder[WRITE]);
    //close(pipefd1[0]); //close read end of pipe
    //dup2(pipefd[WRITE], 1); //Connect pipe to stdout
    //close(pipefd[WRITE]); //close write end
    while(fgets(loadBuffer, MAX_LINE_SIZE, fptr) != NULL)
    {
      //Linebuffer contains new line character
      lineSize = strlen(loadBuffer)-2;
      for(int i = lineSize-1; i >= 0; i--)
      {
        char c = loadBuffer[i];
        printf("WRITE: %c\n", c);
        write(pipeToComplimenter[WRITE], &c, 1);
        signal(SIGUSR1, complimentSigHandler);
        pause();
        sleep(1);

      }
      printf("\n\n");
      // child = waitpid(child, &status, WUNTRACED | WCONTINUED);
    //wait(NULL);
   }
 }
  //printf("%s %s", argv[1], argv[2]);
  /* code */
  return 0;
}

void complimentSigHandler(int sigNum){
  printf("Complimenter receieved byte\n");
}
void incSigHandler(int sigNum){
  printf("Incrementer receieved byte\n");
}

void addSigHandler(int sigNum){
  printf("Adder receieved byte\n");
}
