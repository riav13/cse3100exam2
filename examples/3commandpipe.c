#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>


/*
cat pipe.c | tr [a-z] [A-Z] | tail
cat pipe.c reads the file, tr a-x converts lowercase to uppercase for this file, tail prints last part of output
create pipes - 2 needed 

create child processes

redirect standard input/output

replace each child with the command using exec

Steps: first child process for the cat pipe.c, we want to write this to our pipe.
*/
int main(int argc, char *argv[]) 
{
  int pd[2]; //creating pipe array with 2 ends 
  //pd will be the one that sends cat pipe.c result as input to tr [a-z]
  if (pipe(pd) == -1) //THROWING AN ERROR IF THIS PIPE FAILS
  {
    perror("Error."); //failed making the pipe 
    return -1;
  }

  //SEDNING OUTPUT OF PIPE.C as input to tr a-z A_Z
  pid_t pid = fork(); //creating first child process 
  if (pid == 0) //in the child process we want to access pipe.c for cat command, we want to write this to the pipe 
   {
    close(pd[0]); // closing the reading end of the pipe 
    //REDIRECTION; WE WANT TO REDIRECT THE OUTPUT OF PIPE.C TO THE WRITE END OF PIPE 
    dup2(pd[1], 1); //oldfd , newfd , newfd points to same open file as oldfd we want to replace second arguement with first arguement 
    close(pd[1]); //close the writing end of the pipe 
    //exec replaces the child process with a command so we use execvp 
    char *argv_list[] = {"cat", "pipe.c", NULL};  // cat pipe.c , ends with NULL pointer 
    //child process for each command 
    execvp(argv_list[0], argv_list); // cat pipe.c 
  }
  //now its stored in the write end of pipe 1 
  close(pd[1]); //close the write end of the first pipe 

  //PARENT PROCESS
  //second pipe for connecting the tr to tail 
  int pd1[2]; //creating a second pipe here called pd1 
  if (pipe(pd1) == -1) { //same error 
    perror("Error.");
    return -1;
  }

  //second pipe will take input from the first pipe and send its output into second pipe 

  pid_t pid1 = fork(); // creating a second child process 
  // CHILD 2 will run: tr [a-z] [A-Z] which is converting whatever is in pd[1] from lower to uppercase 
  //reads what cat pipe.c produced from first pipe
  //convert lowercase to uppercase
  //send new output to tail 
  if (pid1 == 0) { //if we are in the child process 
    close(pd1[0]); //close the read end of the second pipe since we need to write into it 
    dup2(pd[0], 0); //take the read end of the first pipe and make that become standard input 
    close(pd[0]); //close the reading end of the first pipe now that we're done 
    //REDIRECT - WE WANT TO REDIRECT THE OUTPUT TO THE WRITE END 
    dup2(pd1[1], 1); //take write end of the second pipe and make it become the standard output 
    close(pd1[1]); //close write end of second pipe 
    char *argv_list[] = {"tr", "[a-z]", "[A-Z]", NULL}; //now we want to run the tr command 
    execvp(argv_list[0], argv_list);
  }
  close(pd[0]);
  close(pd1[1]);


  //3rd child process does tail which prints the last part of the output
  //first we need to read input from the read end of second pipe and make that our output
  pid_t pid2 = fork(); //create a third child process which prints the last part of the output 
  if (pid2 == 0) 
  {
    dup2(pd1[0], 0); // read input from the read end of second pipe and make that our output
    close(pd1[0]); 
    char *argv_list[] = {"tail", NULL};
    execvp(argv_list[0], argv_list);
  }
  close(pd1[0]);

  waitpid(pid, NULL, 0);
  waitpid(pid1, NULL, 0);
  waitpid(pid2, NULL, 0);
  return 0;
}
