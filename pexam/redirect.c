#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

//Goal: want to do the tr a-z A-Z command and use a text file as a input (redirect)
//and also save the resule to a file specified by the user 

// redirect standard input to the specified file
void redirectStdin(const char *filename) {
  int fd = open(filename, O_RDONLY); // fd is the file that we want to open 
  if (fd < 0) {
    perror("Error opening the file\n");
    exit(-1);
  }
  // TODO
  // fill in the code below
  
  //redirect standard input to the specified file
  dup2(fd,0); //our fd file will replace 0 which is standard input 
  close(fd);

  




}

// redirect standad output to the specified file
void redirectStdout(const char *filename) {
  int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  if (fd < 0) {
    perror("Error opening the file\n");
    exit(-1);
  }
  // TODO
  // fill in the code below
  
  //redirect standard input to the specified file
  dup2(fd,1);
  close(fd); //whenever we open one we always have to close it too 

}
