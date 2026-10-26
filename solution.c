#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include<ctype.h>
#include<stdlib.h>

/*
workflow 
P-> C1
C1 -> C2
C2 -> P
P-> C2
C2->C1
C1-> P
*/
/*
step 1 in pd1: parent reads integer from cl arguements (read end of pipe1), writes this to child1 using pipe pd1
step 2: child1 reads value from parent using same pipe pd1, multiplies it with 2 
step 3: sends it to
*/

int main(int argc, char **argv) 
{

    //creating the 6 pipes here 
  int c1p[2]; //child1 -> parent    
  if (pipe(c1p) < 0) //error in creation 
  {
    perror("Error.");
    exit(-1);
  }

  //using this for the 
  int pc1[2]; // parent -> child1
  // pipe creation
  if (pipe(pc1) < 0) {
    perror("Error.");
    exit(-1);
  }

  int c1c2[2]; // pd3: child1 -> child2
  // pipe creation
  if (pipe(c1c2) < 0) {
    perror("Error.");
    exit(-1);
  }

  int c2c1[2]; //pd4: child2 -> child1
  // pipe creation
  if (pipe(c2c1) < 0) {
    perror("Error.");
    exit(-1);
  }

  int c2p[2]; //pd5: child2 -> parent
  // pipe creation
  if (pipe(c2p) < 0) {
    perror("Error.");
    exit(-1);
  }

  int pc2[2]; //pd6 parent -> child2
  // pipe creation
  if (pipe(pc2) < 0) {
    perror("Error.");
    exit(-1);
  }


  //PARENT PROCESS
 char numberforgame=   *argv[1]; //reasig from first command line arguement take the thing in the header 
 //taking an integer as command line arguement integer is called numberfor game 

  int child1= fork(); //creating child 1 
  //waitpid(child1,0,NULL); //wait ing for the child
  



  //CHILD PROCESS 1 
  if(child1==0)
  { //inside child process 1

    /*
    child 1 needs these ones
    -needs to read from the parent pd2 [ 0]
    -needs to write to the child 2 pd3[ 1]
    -needs to read from child 2 pd 4 [ 0]
    -needs to write back to the parent at the end to send th result pd1 [1 write end ]



    //first closing all the file descriptors that are not needed 
    */
    close(c1p[0]); // closing the child1 to parent read end 
    close(pc1[1]); //closing the write end of the parent to child 1 since were reading 

    //child 2 stuff all closed both ends bc were using it in second part 
    close(c1c2[0]); 
    close(c2c1[1]);
    
    close(c2c1[0]);
    close(c2c1[1]);
    //parent to child 2
    close(pc2[0]); //were not even using anything that has to do wijt child 2 here . 
    close(pc2[1]);

    int i; //placeholder for what we want to read 
    read(pc1[0],&i,sizeof(int)); //reads the value of out from parent so use pd2 

    //parent 1 sends to child 1 so we are sending rea
    close(pc1[0]); //close the fd
    printf("\nChild1 read:%d\n",i);
    i*= 2; //multiply it by 2 each time then 4 

    write(c1c2[1],&i,sizeof(int)); //write value to child 2 so we are using pd 3
    close(c1c2[1]);
    printf("\nChild1 wrote:%d\n",i);

    read(c2c1[0],&i,sizeof(int)); //now we want to read from child2 to the parent this value of out 
    close(c2c1[0]); //read it so now we close it
    printf("\nChild1 read:%c\n",i);
     i*= 4; //now times it by 4 for the next one 

    write(c1p[1],&i,sizeof(int)); //now we are going to write this to the parent do use parent to child 1 
    close(c1p[1]); //after sending result to parent 
    printf("\nChild1 wrote:%dn",i);

    exit(0); //child process ends here so rest of stuff is parent process 

  }








  // CHILD 2 PROCESS Closig all the file descriptors that have to do eith child 1
  int child2 = fork();
  // 
  if(child2==0) //child 2 process 
  
  {
    /*
    Child2 only needs these:

 0 read from child1 
 1 write back to parent 
  0 read from parent
  1write to child1

    
    */
    //close all file descriptors that are not needed 
    close(c1c2[1]);
    close(c2c1[0]);
    close(c2c1[0]);
    close(pc2[1]);
    
    //close these 
    close(c1p[0]);
    close(c1p[1]);

    close(pc1[0]);
    close(pc1[1]);

    int i;
    read(c1c2[0],&i,sizeof(int)); //read from child1 to child 2 this part 
    close(c1c2[0]);
    printf("\nChild2 read:%d\n",i);
    i *= 8;


    write(c2c1[1],&i,sizeof(int));
    close(c2c1[1]);


    printf("Child 2 wrote:%d\n",i);
    read(pc2[0],&i,sizeof(int));
    close(pc2[0]);


    printf("Child 2 read:%d\n",i);


    i *= 4; //second to last muktiplication 

    //is 4 
    write(c2c1[1],&i,sizeof(int));
    close(c2c1[1]);
    printf("Child 2 wrote:%d\n",i);

    exit(0); //child process ends here so rest of stuff is parent process 

  }
//OUTSIDE OF parent process close every firle desicrpto we used 

//copy and paste from inside of child process 
  close(c1p[1]); //child 1 to parent write already closed it 
  close(pc1[0]);


  close(c2c1[1]);
  close(pc2[0]);

  //closing the file descepitors thar we alreayd used 
  close(c1c2[0]);
  close(c1c2[1]);

  //child 2 to child 1
  close(c2c1[0]);
  close(c2c1[1]);

 //parent to child process 1 we r gonna write the number game to the pipe parent to child 1 
  write(pc1[1],&i,sizeof(int));
  close(pc1[1]); //close that file des[cr ]
  printf("\nParent wrote: %d\n",i);


  //child 2 to child 1 proces child 1 is gonna recive it from the pipe so read 
  read(c2c1[0],&i,sizeof(int)); //read it from pipe 
  close(c2c1[0]); //close this file descriptor 
  printf("\nParent read:%d\n",i);
  //i *= 2; //last multipi ing muotply it by 2 in the c2 to c1 , insturction

//PARENT PROCESS 
//parent to child2 is gonna receve the integer 
//parent to child 2 parent recieve sinteger multiys it by 4 
  write(pc2[1],&i,sizeof(int)); // write th result back to CHILD 2 SO WE USE PC2 the write end.
  close(pc2[1]);
  printf("\nParent wrote:%d\n",i);
  //i * = 4;

//child  1 to parent. 
  read(c1p[0],&i,sizeof(int)); //reading from child 1 to parnet 
  close(c1p[0]);
  printf("\nParent read:%d\n",i);

  //return exit(EXIT_SUCCESS);
  return 0; 

}
