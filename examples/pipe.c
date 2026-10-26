#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

// Goal: child process will send an integer to the parent process
// child process will communicate with parent process and send the parent
// process an integer

int main() 
{

  //CREATING A PIPE WITH TWO ENDS, TAKES IN ARRAY OF LENGTH 2
  int p[2]; //creates an array of lenght 2 for the two ends of the pipe 
  pipe(p); //creating a pipe with the array for the two ends 

  //Creating a child process here 
  
  int pid = fork(); 

  //CHILD PROCESS
  //inherits the same file descriptors as the parent process, so it has the p and pipe
  if (pid == 0) // if we are in the child process 
  {
    // child process
    // child process will only write, it doesn't read
    close(p[0]); // so it doesn't need to hold the file descriptor for read 
    printf("This is a child process\n");
    int value = 369;
    write(p[1], &value, sizeof(int)); //using write function to write the value to the pipe's write end 
    //takes in address of value 
    close(p[1]);//close the file descriptor RIGHT after writing it bc we don't need it.
    printf("Child process wrote  = %d\n", value);
    exit(0); //child process ends here so rest of stuff is parent process 
  }
  //PARENT PROCESS
  // parent process
  // parent process will only read , it doesn't write
  //    PARENT PROCESS DOESN'T WRITE ANYTHING, CLOSE ITS WRITING END WHICH IS 1 
  close(p[1]); // we are closing the fiel descriptor for writing here 
  int value; //creating a variable for storing the value 
  //READ- BLOCKING, MAKES PROCESS WAIT UNTIL SOME CONTENTS IN THE PIPE
  read(p[0], &value, sizeof(int)); //doing a read function to read the value and store it in file descriptor 0 for read

  //ONCE DONE READING WE NEED TO CLOSE READING FILE DESCRIPTOR 
  close(p[0]); //now closing the file descriptor for reading since we no longer need it 
  printf("parent process read = %d\n", value);
  return 0;
}
