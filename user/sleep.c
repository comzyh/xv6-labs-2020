#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char* argv[])
{

  if(argc == 1) {
    write(2, "sleep need a interger to specific number of ticks.", 50);
    exit(1);
  }
  int tick = atoi(argv[1]);
  sleep(tick);
  exit(0);
}
