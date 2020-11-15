#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char* argv[])
{
  int p2c[2], c2p[2];
  pipe(p2c);
  pipe(c2p);
  if(fork() > 0) { // parent
    uint8 byte_parent = 123;
    if(write(p2c[1], &byte_parent, 1) != 1) {
      fprintf(2, "error write ping\n");
    }
    if(read(c2p[0], &byte_parent, 1) != 1) {
      fprintf(2, "error read pong\n");
    }
    int pid = getpid();
    printf("%d: received pong\n", pid);
  } else { // child
    uint8 byte_child;
    if(read(p2c[0], &byte_child, 1) != 1) {
      fprintf(2, "error read ping\n");
    }
    int pid = getpid();
    printf("%d: received ping\n", pid);
    if(write(c2p[1], &byte_child, 1) != 1) {
      fprintf(2, "error write pong\n");
    }
  }
  exit(0);
}
