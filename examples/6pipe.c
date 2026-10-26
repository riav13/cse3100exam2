#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

// goal:
// create 2 child processes
// both children are children of the parent, not of each other
//
// use 6 pipes total for bidirectional communication:
//
// parent -> child1
// child1 -> parent
//
// parent -> child2
// child2 -> parent
//
// child1 -> child2
// child2 -> child1

int main() {
  // parent <-> child1
  int p_to_c1[2];
  int c1_to_p[2];

  // parent <-> child2
  int p_to_c2[2];
  int c2_to_p[2];

  // child1 <-> child2
  int c1_to_c2[2];
  int c2_to_c1[2];

  // creating all 6 pipes
  pipe(p_to_c1);
  pipe(c1_to_p);
  pipe(p_to_c2);
  pipe(c2_to_p);
  pipe(c1_to_c2);
  pipe(c2_to_c1);

  int pid1 = fork(); // creating child process 1

  if (pid1 == 0) {
    // ================= child process 1 =================
    // child1 will:
    // read from parent
    // write to parent
    // write to child2
    // read from child2

    // keep:
    // p_to_c1[0]
    // c1_to_p[1]
    // c1_to_c2[1]
    // c2_to_c1[0]

    // close unused ends for parent-child1 pipes
    close(p_to_c1[1]);   // child1 does not write to this pipe
    close(c1_to_p[0]);   // child1 does not read from this pipe

    // close all parent-child2 pipe ends since child1 does not use them
    close(p_to_c2[0]);
    close(p_to_c2[1]);
    close(c2_to_p[0]);
    close(c2_to_p[1]);

    // close unused ends for child-child pipes
    close(c1_to_c2[0]);  // child1 does not read from c1_to_c2
    close(c2_to_c1[1]);  // child1 does not write to c2_to_c1

    char buf[100];

    // read message from parent
    read(p_to_c1[0], buf, sizeof(buf));
    printf("Child 1 read from parent: %s\n", buf);

    // write reply to parent
    write(c1_to_p[1], "hello parent from child 1", 26);
    printf("Child 1 wrote to parent\n");

    // write message to child2
    write(c1_to_c2[1], "hello child 2 from child 1", 27);
    printf("Child 1 wrote to child 2\n");

    // read message from child2
    memset(buf, 0, sizeof(buf));
    read(c2_to_c1[0], buf, sizeof(buf));
    printf("Child 1 read from child 2: %s\n", buf);

    // close the pipe ends child1 actually used
    close(p_to_c1[0]);
    close(c1_to_p[1]);
    close(c1_to_c2[1]);
    close(c2_to_c1[0]);

    exit(0);
  }

  int pid2 = fork(); // creating child process 2

  if (pid2 == 0) {
    // ================= child process 2 =================
    // child2 will:
    // read from parent
    // write to parent
    // read from child1
    // write to child1

    // keep:
    // p_to_c2[0]
    // c2_to_p[1]
    // c1_to_c2[0]
    // c2_to_c1[1]

    // close all parent-child1 pipe ends since child2 does not use them
    close(p_to_c1[0]);
    close(p_to_c1[1]);
    close(c1_to_p[0]);
    close(c1_to_p[1]);

    // close unused ends for parent-child2 pipes
    close(p_to_c2[1]);   // child2 does not write to this pipe
    close(c2_to_p[0]);   // child2 does not read from this pipe

    // close unused ends for child-child pipes
    close(c1_to_c2[1]);  // child2 does not write to c1_to_c2
    close(c2_to_c1[0]);  // child2 does not read from c2_to_c1

    char buf[100];

    // read message from parent
    read(p_to_c2[0], buf, sizeof(buf));
    printf("Child 2 read from parent: %s\n", buf);

    // write reply to parent
    write(c2_to_p[1], "hello parent from child 2", 26);
    printf("Child 2 wrote to parent\n");

    // read message from child1
    memset(buf, 0, sizeof(buf));
    read(c1_to_c2[0], buf, sizeof(buf));
    printf("Child 2 read from child 1: %s\n", buf);

    // write message to child1
    write(c2_to_c1[1], "hello child 1 from child 2", 27);
    printf("Child 2 wrote to child 1\n");

    // close the pipe ends child2 actually used
    close(p_to_c2[0]);
    close(c2_to_p[1]);
    close(c1_to_c2[0]);
    close(c2_to_c1[1]);

    exit(0);
  }

  // ================= parent process =================
  // parent will:
  // write to child1
  // read from child1
  // write to child2
  // read from child2
  //
  // parent does not use child1 <-> child2 pipes

  // close unused ends for parent-child1 pipes
  close(p_to_c1[0]);   // parent does not read from this pipe
  close(c1_to_p[1]);   // parent does not write to this pipe

  // close unused ends for parent-child2 pipes
  close(p_to_c2[0]);   // parent does not read from this pipe
  close(c2_to_p[1]);   // parent does not write to this pipe

  // parent does not use child-child pipes, so close all of them
  close(c1_to_c2[0]);
  close(c1_to_c2[1]);
  close(c2_to_c1[0]);
  close(c2_to_c1[1]);

  char buf[100];

  // write to child1
  write(p_to_c1[1], "hello child 1 from parent", 26);
  printf("Parent wrote to child 1\n");

  // write to child2
  write(p_to_c2[1], "hello child 2 from parent", 26);
  printf("Parent wrote to child 2\n");

  // read from child1
  memset(buf, 0, sizeof(buf));
  read(c1_to_p[0], buf, sizeof(buf));
  printf("Parent read from child 1: %s\n", buf);

  // read from child2
  memset(buf, 0, sizeof(buf));
  read(c2_to_p[0], buf, sizeof(buf));
  printf("Parent read from child 2: %s\n", buf);

  // close the pipe ends parent actually used
  close(p_to_c1[1]);
  close(c1_to_p[0]);
  close(p_to_c2[1]);
  close(c2_to_p[0]);

  waitpid(pid1, NULL, 0);
  waitpid(pid2, NULL, 0);

  return 0;
}