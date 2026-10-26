#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/wait.h>

//execl - executes using the full path of the program so using the ./calc 
//execlp - does not need full path and uses the ls command in it to automatically search for it 

int main() 
{
printf("This file will run some other c file\n"); //thats what execl does 
 int a = fork(); //creating a child process of the main process
 if(a==0) //if its the child process that is running then only we want to run this new process 
 {
  //execl simply goes to a path and executes this as as the process 
  //it goes to the path and runs the commmands using parameters 
	execl("./calc", "./calc", "sum", "1", "2", "3", "4", NULL);
  printf("If you can see this the exec failed because after it works, nothing else should");
  //note for path need to compile it first using gcc and use the -o name 
  //1. takes in the path so the -o NAME as first arguement
  //2. takes in the function arguements so like ./calc sum 1 2 3 4
  //3. last thing it takes is NULL, tells it that it is all done 
 }
 else //if its the parent process 
 {
  wait(NULL);
  printf("Done with that other file\n"); //since exec worked properly everything after line 11 won't work
  
 }
	 
  //once exec command is done running successfully works, it takes over the entire current process's memory
  // IF EXEC WORKED PROPERLY, EVERYTHING AFTER LINE 11 WILL NOT WORK
  //SUCCESSFUL EXEC NEVER RETURNS, ONCE ITS DONE EVERYTHING UNDER IT WON'T WORK

  return 0;
}
