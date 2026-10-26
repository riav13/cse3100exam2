#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

//process 1 = main parent
//a = fork(), process 1 and process 2 (child)
//parent process process 1 called b= fork() and then it creates another process 
int main() 
{
    //creating two separate child processes 
  int a = fork(); 
  int b = fork();

  printf("a = %d,b=%d,pid = %d\n", a, b, getpid());
  if (a > 0 && b > 0) //the ultimate parent process for both of them
  //greater than 0 represents a parent 
  {
		waitpid(a,NULL,0); //waits for 
		waitpid(b,NULL,0);
    printf("The main process 1, the god father\n");
  }
	if(a==0) //if both are childs //if a is a child 
    {
		if(b==0) //and if b is a child too 
        {
			printf("This is process 4\n");
		}
		if(b>0) //if the b is a parent 
        {
			printf("This is process 2\n");
		}
	}
	if(a>0 && b==0) // if a is a parent and b is a child 
    {
		printf("This is process 3\n");
	}	
  return 0;
}

//2 forks means there are 4 processes
