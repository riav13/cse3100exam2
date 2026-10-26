#include <stdio.h> 
#include <sys/wait.h> //for the wait function in processes 
#include <unistd.h> //for forking in processes 
//Process: anything that runs, running code called pro.c
//fork() creates a child process
int main() 
{
  //EVERYTHING STARTING AFTER MAIN COUNTS AS THE PARENT PROCESS 
  printf("This is a demonstration\n"); //this is the original parent process 
  int a = fork(); //this creates a child process, now there are 2 processes: parent and child 
  //both of these processes are now continuing to run the next lines at the same time 
 
  //WHAT DOES FORK() RETURN?
  // for the child process, fork returns 0 
  //for the parent process, fork returns the pid of the child (positive number)
  //if error in fork, it returns -1
 
  //RUNS ONLY IF ITS THE CHILD PROCESS 
  if (a == 0)  //if its the child process 
  {
    printf("This is in child process\n"); //print this 
    printf("Child process id = %d\n", getpid()); //getpid() returns pid of the calling process 
  }
  //RUNS ONLY IN THE PARENT SINCE ITS THE PID OF THE CHILD 
  if (a > 0) //if its the parent process meaning it returns the pid (positive number)
  {
    printf("This is in parent process\n"); //prints out this is a parent process
    //WAIT FOR THE CHILD PROCESS (WHOS PID IS STORED IN "a") TO FINISH
    //WAIT - PARENT PAUSES UNTIL CHILD FINISHES 
    waitpid(a, NULL, 0); //wait for the child process whos pid is stored in a to finish before continuing with parent 
    //takes child's process id,status = null, option 0 
    printf("Parent process id = %d\n", getpid()); //now its gonna print out the pid of the current process
    //this prints out the pid of the PARENT process 
  }

  printf("Bye!\n"); //now this one is gonna print twice, since its outside of both if blocks 
  return 0;
}


//STEPS
//starts with parent process running
//fork() creates a new process 
//is a=0 in parent? no so it runs whats in the loop for a>0
//
//waitpid - parent process should wait for child process is done
