// Parent process will send an integer n
// Child process will return sum of n natural numbers
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
// in child process
// child process will recieve value n so it reads from the pipe 
// it will then compute sum of n natural number
// it will write the result to parent

// parent process
// parent will send /write value n to the pipe 
// it will recieve the sum of n natural numbers from child after


//STEPS
/*
1.parent process writes integer n to pipe
2.child process reads integer n from pipe, computes sum using for loop
3.child process will write the sum to the new pipe 
4.parent process will read the sum from the new pipe and done. 


*/

int main() 
{

  //BI DIRECTIONAL = needs two pipes, 1 pipe for child to parent, another for parent to child 
  int pc[2]; // use for parent(write)---> child(read) pc[1] ---> pc[0];
  //pc is from parent to child 
  int cp[2]; // use for child(write)---> parent (read) cp[1] ---> cp[0];
  //child to parent 

  pipe(pc); //parent to child communication
  pipe(cp); //child to parent communication

  //CHILD PROCESS- READS INTEGER N FROM PIPE 
  //use the parent to child pipe 
  int pid = fork(); //creating our child process 
  if (pid == 0) 
  {
    // child process
    // child process will first read from pc[0], read value n, it will never write to pc[1]
    close(pc[1]); //close writing end 
    int n; //initializing integer n 
    read(pc[0], &n, sizeof(int)); //now we read the value of n into the read end of parent to child pipe 
    //it reads from pc[0] the read end, it reads n, the size of it is the size of an integer 
    printf("n = %d\n", n); 
    close(pc[0]); //make sure to close the read end because we are done with it 
    int res = (n * (n + 1)) / 2; //sum of n natural numbers formula 
    // child process will write / send result to parent, it will write to cp[1],
    // it never reads from cp[0];
    close(cp[0]); //we are closing the reading end since the child process is writing to parent 
    write(cp[1], &res, sizeof(int)); //writes it to the write end of child process pipe, result, sizeofint 
    close(cp[1]); //now we are done writing so close the write end 
    exit(0); //exits the child process 
  }
  // PARENT PROCESS 1ST THING IS IT WRITES N TO THE CHILD 
  //first writes the n to child, so we use parent to child pipe pc pipe 
  // write / send value n to child, it writes to pc[1], it never reads from
  // pc[0]
  close(pc[0]); //closing the reading end of the pipe first
  int n = 5; //setting the n value to be 5 
  write(pc[1], &n, sizeof(int)); //write this value to the WRITE end of pipe pc
  close(pc[1]); //closing the write end because we are done 


  //PARENT PROCESS NOW RECIEVES THE SUM OF N NATURAL NUMBERS FROM THE CP PIPE 
  int res; //creating variable to store result 
  // reads from child, reads from cp[0], it never writes to cp[1];
  close(cp[1]); //closing the writing end of cp since we are only using reading end 
  read(cp[0], &res, sizeof(int)); //we want to read in the value from read end, store it in result
  close(cp[0]); //close the read end now that we're done 
  printf("result = %d\n", res);
  return 0;
}
