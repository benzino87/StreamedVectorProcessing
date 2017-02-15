/**
 *
 * AUTHOR:        Jason Bensel
 * DESCRIPTION:   452 - Project 2 Streamed Vector Processing
 *                2/13/2016
 *
 *                Reads a file and pipes it to three seperate processes, the
 *                first process "Complimenter" inverts the incoming bits and
 *                pipes it to the the second process.
 *                The scond process "Incrementer" adds one to each bit from pipe
 *                adds one to each bit, creating two's compliment, then pipes it
 *                to the third process.
 *                The third process "Adder" reads from the pipe and from file b,
 *                adds the bits together performing the binary subtraction,
 *                it then writes each character to a buffer which is then saved
 *                to a file.
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
#define TRUE 1
#define FALSE 0
#define MAX 64
#define MAX_FILE_SIZE

void sigHandlerGeneral(int);
void sigFromCompliment(int);

int main(int argc, char const *argv[]){

  /** PIDS for all processes **/
  pid_t child_complimenter, child_incrementer, child_adder;
  /** Pipe 1 **/
  int pipeToComplimenter[2];
  /** Pipe 2 **/
  int pipeToIncrementer[2];
  /** Pipe 3 **/
  int pipeToAdder[2];

  size_t num;
  char *str;
  //File read pointer and file write pointer
  FILE *fptr;
  FILE *fwptr;
  size_t linesize;
  char* loadBuffer;
  char* saveBuffer;
  loadBuffer = malloc(MAX);
  saveBuffer = malloc(MAX);
  str = malloc(sizeof(char));


  //Create pipes
  if(pipe(pipeToComplimenter) == -1){
    printf("Complimenter pipe error");
    exit(1);
  }
  if(pipe(pipeToIncrementer) == -1){
    printf("Incementer pipe error");
    exit(1);
  }
  if(pipe(pipeToAdder) == -1){
    printf("Adder pipe error");
    exit(1);
  }

  //Create forks
  if((child_complimenter = fork()) == -1){
    perror("Complimenter fork error");
    exit(1);
  }
  /** COMPLIMENTER **/
  if(child_complimenter == 0){

    if((child_incrementer = fork()) == -1){
      perror("Complimenter fork error");
      exit(1);
    }
    /** INCREMENTER **/
    if(child_incrementer == 0){

      if((child_adder = fork()) == -1){
        perror("Complimenter fork error");
        exit(1);
      }

      /** ADDER **/
      if(child_adder == 0){
	fprintf(stderr, "Adder created...\n");
        pid_t ppid = getppid();
        char *buf;
        char writeChar;
        int hasCarry = FALSE;

        //Redirect input and output
        dup2(pipeToAdder[READ], STDIN_FILENO);

        //Close all unused pipes
        close(pipeToComplimenter[WRITE]);
        close(pipeToComplimenter[READ]);
        close(pipeToIncrementer[WRITE]);
        close(pipeToIncrementer[READ]);
        close(pipeToAdder[WRITE]);
        close(pipeToAdder[READ]);

        //Open file for writing
        if((fwptr = fopen("output.dat", "w")) == NULL){
          perror("Error opening write file");
          exit(1);
        }

        //Read from second file
        if((fptr = fopen(argv[2], "r")) == NULL){
          perror("Error opening read file");
          exit(1);
        }

        /**
          *
          * -Begin adder process by loading in file B one line at a time
          *   and accepting bits from pipe.
          *
          * -Perform standard error checking and add the bits together.
          *
          * -Write the bits to a save buffer and save the file.
          *
          */
        while(getline(&loadBuffer, &linesize, fptr) != -1){

          for(int i = sizeof(loadBuffer)-1; i >= 0; i--){
            buf = &loadBuffer[i];

            if((num = read (STDIN_FILENO, str, 1)) > 0){
              if (num > MAX) {
                perror ("pipe read error\n");
                exit(1);
              }
              if(*str == 'n'){
                if((num = read (STDIN_FILENO, str, 1)) > 0){
                  if (num > MAX) {
                    perror ("pipe read error\n");
                    exit(1);
                  }
                }
                hasCarry = FALSE;
              }

              //Handle bit addition from pipe and load buffer
              if(*str != 'n'){

                if(*str == '0' && buf[0] == '0' && hasCarry == FALSE){
                  writeChar = '0';
                }
                else if(*str == '1' && buf[0] == '0' && hasCarry == FALSE){
                  writeChar = '1';
                }
                else if(*str == '0' && buf[0] == '1' && hasCarry == FALSE){
                  writeChar = '1';
                }
                else if(*str == '1' && buf[0] == '1' && hasCarry == FALSE){
                  writeChar = '0';
                  hasCarry = TRUE;
                }
                else if(*str == '0' && buf[0] == '0' && hasCarry == TRUE){
                  writeChar = '1';
                  hasCarry = FALSE;
                }
                else if(*str == '0' && buf[0] == '1' && hasCarry == TRUE){
                  writeChar = '0';
                  hasCarry = TRUE;
                }
                else if(*str == '1' && buf[0] == '0' && hasCarry == TRUE){
                  writeChar = '0';
                  hasCarry = TRUE;
                }
                else if(*str == '1' && buf[0] == '1' && hasCarry == TRUE){
                  writeChar = '1';
                  hasCarry = TRUE;
                }
                saveBuffer[i] = writeChar;
              }
            }
          }
          fputs(saveBuffer, fwptr);
          fputc('\n', fwptr);
        }

        fclose(fptr);
        fclose(fwptr);

      }
      else{
	fprintf(stderr, "Incrementer created...\n");
        pid_t ppid = getppid();
        int isFirstIteration = TRUE;
        int hasCarry = FALSE;
        int count = 0;

        //Redirect input and output
        dup2(pipeToIncrementer[READ], STDIN_FILENO);
        dup2(pipeToAdder[WRITE], STDOUT_FILENO);

        //Close all unused pipes
        close(pipeToComplimenter[WRITE]);
        close(pipeToComplimenter[READ]);
        close(pipeToIncrementer[WRITE]);
        close(pipeToIncrementer[READ]);
        close(pipeToAdder[WRITE]);
        close(pipeToAdder[READ]);



        while((num = read (STDIN_FILENO, str, 1)) > 0){
          if (num > MAX) {
            perror ("pipe read error\n");
            exit(1);
          }

          if(*str == '0' && isFirstIteration == TRUE){
            *str = '1';
            hasCarry = FALSE;
            isFirstIteration = FALSE;
          }

          //Add one and set carry if first inverted bit is 0
          else if(*str == '1' && isFirstIteration == TRUE){
            *str = '0';
            hasCarry = TRUE;
            isFirstIteration = FALSE;
          }

          //Do not add if no carry in
          else if(*str == '0' && isFirstIteration == FALSE && hasCarry == FALSE){
            *str = '0';
            hasCarry = FALSE;
            isFirstIteration = FALSE;
          }

          //Add one if carry in
          else if(*str == '0' && isFirstIteration == FALSE && hasCarry == TRUE){
            *str = '1';
            hasCarry = FALSE;
            isFirstIteration = FALSE;
          }

          //Do not add if no carry in
          else if(*str == '1' && isFirstIteration == FALSE && hasCarry == FALSE){
            *str = '1';
            hasCarry = FALSE;
            isFirstIteration = FALSE;
          }

          //Add one and keep carry
          else if(*str == '1' && isFirstIteration == FALSE && hasCarry == TRUE){
            *str = '0';
            hasCarry = TRUE;
            isFirstIteration = FALSE;
          }

          //RESET FIELDS
          else if(*str == 'n'){
            hasCarry = FALSE;
            isFirstIteration = TRUE;
            count = 0;
          }
	  //fprintf(stderr, "Increment bit: %s\n", str);
          write(STDOUT_FILENO, str, 1);
        }
      }
    }
    else{
      fprintf(stderr, "Complimenter created...\n");
      pid_t ppid = getppid();
      //printf("Setting up pipes and processes");
      //pause();
      //signal(SIGINT, sigHandlerGeneral);
      //kill(ppid, SIGUSR1);
      //Redirect input and output
      //dup2(pipeToComplimenter[READ], STDIN_FILENO);
      dup2(pipeToIncrementer[WRITE], STDOUT_FILENO);

      //Close all unused pipes
      close(pipeToComplimenter[WRITE]);
      close(pipeToComplimenter[READ]);
      close(pipeToIncrementer[WRITE]);
      close(pipeToIncrementer[READ]);
      close(pipeToAdder[READ]);
      close(pipeToAdder[WRITE]);
      

      
    if((fptr = fopen(argv[1], "r")) == NULL){
      perror("Error opening file");
      return -1;
    }

    while(getline(&loadBuffer, &linesize, fptr) != -1){

      //Reverse the buffer to write backwards
      for(int i = sizeof(loadBuffer)-1; i >= 0; i--){
        str = &loadBuffer[i];
	if(*str == '0'){
          *str = '1';
        }
        else if(*str == '1'){
          *str='0';
        }	
	write(STDOUT_FILENO, str, 1);
	if(i == 0){
	  write(STDOUT_FILENO, "n", 1);
	}
	
	//fprintf(stderr, "Number Read: %c      Compliment: %c\n", loadBuffer[i], str[0]);
      }
    }
  }
  }



  /** PARENT **/
  else{
    //signal(SIGUSR1, sigFromCompliment);
    //Give children time to initialize
    //sleep(1);
    //Redirect complimenter pipe to STDOUT
    //dup2(pipeToComplimenter[WRITE], STDOUT_FILENO);

    //Close all unused pipes
    close(pipeToIncrementer[WRITE]);
    close(pipeToIncrementer[READ]);
    close(pipeToAdder[WRITE]);
    close(pipeToAdder[READ]);
    
    
  }
}
void sigHandlerGeneral(int sigNum){
  printf("Control-C received, ready to process...\n");
}

void sigFromCompliment(int sigNum){
  printf("Processing Data");
}
void sigFromIncrementer(int sigNum){
  printf("Increment received byte\n");
}
void sigFromAdder(int sigNum){
  printf("Adder received byte\n");
}
