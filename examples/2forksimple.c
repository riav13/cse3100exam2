#include <stdio.h>
#include <unistd.h>

int main() 
{
  int a = fork(); //creates a child process which has 2 processes - P1 parent and P2 child 
  int b = fork(); //creates a child process P3, which is clone of P1, and child process P4 child of P2
  if (a == 0) 
  {

		printf("a is child %d\n",a);
		printf("b is child %d\n",b);
    printf("This is in child,PID:%d\n",getpid());
  }
	if(a>0) 
  {
		printf("a is parent %d\n",a);
		printf("b is parent %d\n",b);
    printf("This is in parent,PID:%d\n",getpid());
  }
}
