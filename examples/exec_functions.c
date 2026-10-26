#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

//execl - executes program using exact path takes in path arguements null
//execlp - used when we don't know where the path is and search it automatically (ls)
//execv - used for when we don't know how many arguements we are passing through command line, make a argv ** pointer and list out arguements in terminal 
//execvp - 
int main(int argc, char **argv) 
//the **argv is the pointer to the pointer of the arguements, for execv 
{

  printf("This file will run some other c file\n");
  int a = fork();
  if (a == 0) 
  {
    /* execl("./calc", "./calc", "sum", "1", "2", "3", "4", NULL); //this one runs calc

    /* execlp("ls","ls","-l",NULL); */ // this one would show the permissions and details of file 

    /* char *args[] = {"./calc", "sum", "1", "2", "3", "4", NULL}; */ //here we put all the arguements into one array
    //this is for using the method execv

    /*execv("./calc", &argv[1]); */ //first arguement is still the path
    //the argv represents our arguements when we don't know how many
    //input them like ./calc sum 1 2 3 into terminal after running 

    //char *args[] = {"ls", "-l", NULL}; 
    //execvp(args[0],args);
    execvp(argv[1], &argv[1]); //using the argc pointer we created in the header

    printf("If you can see this, then exec failed\n");
  } 
  else 
  {
    wait(NULL);
    printf("Done with that other file\n");
  }

  return 0;
}
