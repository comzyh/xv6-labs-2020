//
// File-system system calls.
// Mostly argument checking, since we don't trust
// user code, and calls into file.c and fs.c.
//

#include "types.h"
#include "param.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "sysinfo.h"
#include "defs.h"

// return a pointer of
uint64
sys_sysinfo(void)
{
  struct proc *p = myproc();
  uint64 addr;
  if (argaddr(0, &addr) < 0)
    return -1;
  struct sysinfo info;
  info.freemem = kfreemem();
  info.nproc = nproc();
  if (copyout(p->pagetable, addr, (char *)&info, sizeof(info)) < 0)
    return -1;

  return 0;
}