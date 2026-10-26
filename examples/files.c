#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
//FILE IO
//int open(path,flags,mode)
int main() 
{
  //OPENING METHOD 
  int fd = open("test.txt", O_RDWR | O_APPEND); //this makes sure when we write it doesn't replace anything 
    //this one we set it to  read write or append
      //takes in file path, flags
  //O_RDONLY read only, O_WRONLY write only, O_RDWR both read and write 
  //O_APPEND will also append to the contents 

  //READING METOHD - needs a buffer.
  char buf[100]; //making the buffer for reading the file 
  read(fd, buf, 100); //file descriptor, buffer, number of bytes its trying to read so here we say 100 
  //it will read a maximum of 100 bytes 
  //read(0,buf,10); now it will print out what the user entered since it reads from sdtin 0
  printf("%s\n", buf);

  /* read(fd, buf, 10); */ 
  /* printf("%s\n", buf); */


  //WRITE is when we want to write something to file
  //file descriptor, string, bytes it is writing
  char str[] = "Hello\n"; //this is the string we want to write
  int v = write(fd, str, strlen(str)); //write takes in the file descriptor, the string we need to read, and size of it 
  /* printf("%d\n", v); */
  int fd2 = open("test.c",O_RDWR|O_CREAT,0754);
  //opening the file test.c in read write mode or create the file, 

  //ALWAYS CLOSE FILE DESCRIPTORS AFTER OPENING 
  close(fd); //closing the file 
  close(fd2); //closing the file 
}