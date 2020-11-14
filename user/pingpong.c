#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char* argv[])
{
  int p[2];
  pipe(p);
  if(fork() > 0) { // parent
    uint8 byte_parent = 0;
    write(p[0], &byte_parent, 1);
    read(p[0], &byte_parent, 1);
    int pid = getpid();
    printf("%d: received pong\n", pid);
  } else { // child
    uint8 byte_child;
    read(p[1], &byte_child, 1);
    int pid = getpid();
    printf("%d: received ping\n", pid);
    write(p[1], &byte_child, 1);
  }
  exit(0);
}
