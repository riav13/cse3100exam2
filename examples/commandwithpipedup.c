#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

// goal: cat doc.txt | tr a-z A-Z
//run cat's doc.txt and pipe it to tr a-z A-Z
//
//

int main() 
{

  int p[2]; // set up pipe's array
  //PIPE PROCESS ALWAYS RETURNS AN INTGER VALUE
  //-1 MEANS THERE WAS AN ERROR CREATING PIPE, FAILED 
  int ret = pipe(p); //pipe(p)
  if (ret == -1) 
  {
    perror("pipe failed\n");
  }

  int pid1 = fork(); //first child process for the cat part 
  if (pid1 == 0)  // here we are only writing
  {
    close(p[0]); //close the read end of the pipe 
    //trying to exec to cat.txt, trying to get in that file 
    //  NEED A REDIRECTION HERE BECAUSE WE WANT IT TO WRITE TO PIPE INSTEAD OF STDOUT
    //  REDIRECTING STANDARDOUT TO THE PIPE
    dup2(p[1], 1); //dup 2 takes in oldfd and newfd
    //closes 1 that is stdout file descriptor and use the p[1]
    //we want to copy the first arguement ot eh second 
    //replace the second arguement with first
    close(p[1]); //now we will close the writing fd
    //using execlp here to open the cat one 
    execlp("cat", "cat", "pipe.txt", NULL); // cat writes to stdout, but we want it to write to pipe
    exit(1);
  }

  int pid2 = fork(); //second child process is only reading 
  if (pid2 == 0) {
    close(p[1]); //we are going to close the write end of the pipe since its only reading 
    //pr reads from stadnard input, but we want to make it read to the pipe
    dup2(p[0], 0); //oldfd,newfd - newfd gets replaced by oldfd , we want the pipe to replace standard input 
    close(p[0]); //CLOSE THEM AFTER DONE 
    //using execlp to open the path A-Z
    //NEED DUP2 TO MAKE IT READ FROM THE PIPE INSTEAD OF STANDARD INPUT 
    execlp("tr", "tr", "a-z", "A-Z", NULL);
    exit(0);
  }

  close(p[0]);
  close(p[1]);

  //NEED A WAITPID HERE 
  //wait pid is used by parent process to make sure that the two child processes run first 
  waitpid(pid1, NULL, 0);
  waitpid(pid2, NULL, 0);

  return 0;
}
