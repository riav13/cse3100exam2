//parent process will send an integer n
//child process will return sum of n natural numbers
//use two pipes, 1 to send the integer n to child process
//other to return the sum of n natural numbers back into the other pipe.
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

//goal here:
//parent process sends integer n to child process
//child process returns sum of n natural numbers

/*
child
1. child process will read val n from parent
2. child process will compute sum of n natural numbers
3. child will write result back to parent 
parent
1. parent process will write/send val n to child
2.parent process will recieve result/sum from child 

1 PIPE - child to parent
PIPE 2- parent to child 

*/

int main()
{
//PARENT PROCESS
    int p1[2]; //initializing first pipe for parent to child 
    // parent - write, child - read 
    pipe(p1); //p1 is for pipe1

    int p2[2]; //intiailizing second pipe for child to parent 
    //parent - read, child - write 
    pipe(p2); //p2 is the second pipe we will use 


    //PARENT PROCESS SENDING N TO CHILD P1
    int n = 5; //saving ghe integer n 
    //parent writes to child , so we use p1
    close(p1[0]); //since we are not reading
    write(p1[1], &n, sizeof(int));
    close(p1[1]); //once its done using this fd, close it as well

    //CLOSE ALL FDS THAT ARENT NEEDED
    close(p1[0]); //close reading end of pipe since parent process is writing to pipe
    write(p1[1],&n,sizeof(int)); //now we wrote the integer n to write end of pipe1
    close(p1[1]); //closing the write end of pipe bc were done with it now in p process 

    //CHILD PROCESS
    int pid =fork();
    if(pid == 0) //if we are in child process we want it to read from write end of pipe
    { 
        //we are only reading wht integer n is so were gonna close the write end 
        close(p1[1]); //will not write 
        //goal: send integer n from parent process to child process 
        int n; 
        read(p1[0], &n, sizeof(int)); //reads n from the read end , stores it in p[0]
        //for loop to do sum of n natural numbers 
        int sum = 0;
        for(int i=0, i<= n, i++)
        {
            sum += i;
        }
        return sum;
        close(p1[0]); //opened it so now we have to close it the read end
        //now we have to write the sum back to a new pipe. 
        //question, do where do I create this new pipe can I do it right here or above in parent process.
            //create new pipe2 in the parent process so that both parent child can use it 
        //NEXT STEP, WRITE THIS SUM TO THE WRITE END OF PIPE 2. to send to parent 
        close(p2[0]); // close the uneeded fd, we don't need the read end of pipe 2 since child only writes value 
        write(p2[1],&sum, sizeof(int)); //wrote it to new pipe2
        close(p2[1]);
        
        exit(0); //exits child process so now we can finish in parent process 
        
    }

    //FINAL STEP; PARENT PROCESS WE WANT TO READ WHATS IN THE WRITE END OF PIPE2.
    int sum; 
    close(p2[1]); // we are not writing here 
    read(p2[0],&sum,sizeof(int)); //reading it into the READ end of the pipe 
    //so we use the read end in read function
    close(p2[0]); //close it after we're done .






}