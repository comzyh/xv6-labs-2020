#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char* argv[])
{
  int p[2];
  pipe(p);
  if(fork() > 0) { // root
    close(p[0]);   // if this close is removed, the progrem can still exit.
    for(int i = 2; i <= 35; i++) {
      write(p[1], &i, sizeof(int));
    }
    close(p[1]); // MUST close this fd to tell the child no more data is comming
    wait((int*)0); // our root process is guaranteed to have only 1 child. we
                   // should wait for it to exit and then exit root.
  } else {         // first output child
    // we MUST close the write side in child process now, otherwise the p may be
    // overwirte, and we will not have change to close it.
    close(p[1]);
    int prime = 0; // current prime number, initially set to 0 indicate that
                   // prime number is not set
    int parent_pipe_read = p[0]; // the read fd for parent to current pip
    int n;                       // store the value read from parent
    int child_creaded = 0;       // next processing process created
    int r;
    while(r = read(parent_pipe_read, &n, sizeof(int)), r > 0) {
      if(prime == 0) { // prime not readed
        prime = n;
        printf("prime %d\n", prime);
        continue;
      }
      if(n % prime == 0) { // ignore non-prime
        continue;
      }
      if(child_creaded == 0) { // created new process
        pipe(p);               // create new pipe
        if(fork() == 0) {      // child, reset all context
          prime = 0;
          // the p will be replace in next `pipe` call, we MUST close it now
          close(p[1]);
          parent_pipe_read = p[0];
          child_creaded = 0;
          continue; // all context set, read value from parent
        } else {
          child_creaded = 1;
          // interesting, if following close is removed, the program ca still
          // successfully exit.
          close(p[0]);
        }
      }
      write(p[1], &n, sizeof(int)); // pass n to child
    }
    if(child_creaded) {
      close(p[1]); // MUST close the write side to child, to tell child it's end
      wait((int*)0); // we MUST wait for child to exit
    }
    close(parent_pipe_read); // close the read side to parent
  }
  exit(0);
}
// Q: When will a read from pipe returns 0?
// A: all write side of this pipe is closed, include all of its' copy.
//    no mater before fork(parent) or after fork(child).
//    which means we must not forget to close the write side of pipe in child