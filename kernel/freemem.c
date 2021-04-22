#include "types.h"
#include "param.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"

// return a pointer of
uint64
sys_freemem(void)
{
  return kfreemem();
} 