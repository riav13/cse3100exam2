#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
/*
cat pipe.c | tr [a-z] [A-Z] | tail 
cat pipe.c reads the file, tr a-x converts lowercase to uppercase for this file, 
tail prints last part of output
*/
int main(int argc, char *argv[]) {
  int pd[2];
  if (pipe(pd) == -1) {
    perror("Error.");
    return -1;
  }

  //write the result of pipe.c to pd [ 1]
  //PROCESS 1 - CAT PIPE.C
  pid_t pid = fork();
  if (pid == 0) {
    close(pd[0]); //close reading end since we are writing 
    dup2(pd[1], 1); //redirect the output to go to the write end 
    close(pd[1]);
    char *argv_list[] = {"cat", "pipe.c", NULL};
    execvp(argv_list[0], argv_list);
  }
  close(pd[1]);

  int pd1[2];
  if (pipe(pd1) == -1) {
    perror("Error.");
    return -1;
  }

  pid_t pid1 = fork();
  
  //PROCESS 2 tr a-z A_Z
  if (pid1 == 0) {
    close(pd1[0]);
    dup2(pd[0], 0); // want to replace the input with the read end of pipe which has pipe.c result
    //so know the standard input will come from pipe's read end which has cat pipe.c result  
    close(pd[0]);
    dup2(pd1[1], 1); //want the ouput of tr a-z A-Z command to write to write end of next pipe 
    close(pd1[1]);
    char *argv_list[] = {"tr", "[a-z]", "[A-Z]", NULL};
    execvp(argv_list[0], argv_list);
  }
  close(pd[0]);
  close(pd1[1]);

  //PROCESS 3 : 
  pid_t pid2 = fork();
  if (pid2 == 0) {
    dup2(pd1[0], 0); // reads whatever was in the ouput for last pipe, so read end 
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
