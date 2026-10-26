#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

// goal: create two child processes, make child process 1 send hello to child
// process 2
//  child process 1 is only writing, it doesn't read
//  child process 2 is only reading, it doesn't write
//  parent process is not communicating with any child processes

int main() {
  int p[2]; //initializing the array for the pipe 
  pipe(p); //creating the pipe 

  int pid1 = fork(); //creating child process 1 
  if (pid1 == 0) {
    // child process 1
    // will only write
    // doesn't read
    close(p[0]); //closing the reading fd
    write(p[1], "hello", 5); //writing to the write end , writing hello, 5 bytes 
    close(p[1]); //now we have to close the writing fd
    printf("Child process 1 is writing hello\n");

    exit(0);
  }

  int pid2 = fork(); //creating child process 2
  if (pid2 == 0) {
    // child process 2
    // will only  read
    // doesn't write
    close(p[1]); //closing the writing end for child process
    char buf; //here we don't know how many bytes child process 2 needs to read so we make a buf 
    printf("child process 2 is reading:");
    while (read(p[0], &buf, 1) > 0) //reads byte by byte putting it inside of the byte 
    {
      printf("%c", buf);
    }
    printf("\n");
    close(p[0]); //always close since we are done reading 
    exit(0);
  }

  // parent process we are not using write or read end of pipe so we are going to close both ends 
  close(p[0]);
  close(p[1]);
  return 0;
}
