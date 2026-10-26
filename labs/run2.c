#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char ** argv)
{
    pid_t child; //creating a pid for the child process
    int exitStatus; //exitStatus of the pid

    // at least, there should be 3 arguments
    // 2 for the first command, and the rest for the second command
    if (argc < 4) 
    {
        fprintf(stderr, "Usage: %s cmd1 cmd1_arg cmd2 [cmd2_args ..]\n", argv[0]);
        return 1;
    }

    // TODO
    //CHILD PROCESS #1 DOING FIRST EXECLP METHOD 
    child = fork(); //creating a  child process for the first process 
    if(child < 0) //if there is an error 
    {
        perror("fork()"); //return fork error 
        return 1;
    }
    //use excelp to run the first command
    
    if(child ==0) //if its in this child process 
    {
        //then we wanna do execlp command 
        execlp(argv[1], argv[1],argv[2], NULL); //first one is ./run2 
        perror("execlp()"); //throw a p error, if the execlp method runs successfully, will just ignore this 
    }
    waitpid(child,&exitStatus,0); //we use waitpid when we want the parent to pause until child is done 
    printf("exited=%d exitstatus=%d\n", WIFEXITED(exitStatus),WEXITSTATUS(exitStatus));


    child = fork(); // creating
    //  a second fork for the second child process
    if(child < 0) //if there is an error 
    {
        perror("fork()"); //return fork error 
        return 1;
    }
    //use excelp to run the first command
    
    if(child ==0) //if its in this child process 
    {
        //then we wanna do execvp command when we don't know how many arguements we have
        execvp(argv[3], &argv[3]); //& gives us everything after argv[3] 
        perror("execvp()"); //throw a p error, if the execlp method runs successfully, will just ignore this 
    }
    waitpid(child,&exitStatus,0); //we use waitpid when we want the parent to pause until child is done 
    printf("exited=%d exitstatus=%d\n", WIFEXITED(exitStatus), WEXITSTATUS(exitStatus));


    return 0;
}
