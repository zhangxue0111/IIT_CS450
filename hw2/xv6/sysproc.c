#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "syscall.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

static char *syscallsname[] = {
[SYS_fork]      "fork",
[SYS_exit]      "exit",
[SYS_wait]      "wait",
[SYS_pipe]      "pipe",
[SYS_read]      "read",
[SYS_kill]      "kill",
[SYS_exec]      "exec",
[SYS_fstat]     "fstat",
[SYS_chdir]     "chdir",
[SYS_dup]       "dup",
[SYS_getpid]    "getpid",
[SYS_sbrk]      "sbrk",
[SYS_sleep]     "sleep",
[SYS_uptime]    "uptime",
[SYS_open]      "open",
[SYS_write]     "write",
[SYS_mknod]     "mknod",
[SYS_unlink]    "unlink",
[SYS_link]      "link",
[SYS_mkdir]     "mkdir",
[SYS_close]     "close",
[SYS_countTraps]  "countTraps",
};


int hardware_interrupts = 0;
int software_interrupts = 0;

int
sys_countTraps(void)
{
  int mask;

  if(argint(0, &mask) < 0) {
    return -1;
  }

  struct proc *curproc = myproc();
  curproc->mask = mask;
  // cprintf("mask = %d\n", mask);
  if(mask == 0) {
    for(int i = 0; i< 23; i++) {
      if(curproc->syscall_trap_count[i] > 0) {
        cprintf("%d: syscall %s -> executes %d times.\n", curproc->pid, 
          syscallsname[i], curproc->syscall_trap_count[i]);
      }
    }
  } else{
    cprintf("%d: syscall %s -> executes %d times.\n", curproc->pid, 
      syscallsname[mask], curproc->syscall_trap_count[mask]);
  }

  // print the total number of traps caused by system calls.
  cprintf("total system calls -> %d\n", curproc->total_syscalls);
  // print the total number of traps caused by hardware interrupts.
  cprintf("total hardware interrupts -> %d\n", hardware_interrupts);
  // print the total number of traps caused by exceptions.
  cprintf("total exceptions -> %d\n", software_interrupts);

  return 0;
}
