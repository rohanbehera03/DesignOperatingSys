#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(int status)
{
    argint(0, &status); //pass an integer into the kernel(pos. 0)
    exit(status);
    return 0; // not reached
}

int
sys_wait(void)
{
    int *status;
    if(argptr(0, (void*)&status, sizeof(status)) < 0) { //check for valid status
        //waits for child to finish executing, then goes back to parent to finish the process
        return -1; //no child exists
    }
  return wait(status); //return the terminated child exit status by passing
                        // in the status pointer to wait function
}

int
sys_waitpid(void) {
    int pid;
    // pass argument into kernel function using argint
    // get first pid(index 0) if its <0, return -1
    if (argint(0, &pid) < 0){
        return -1;
    }
    int *status;
    //system call as ptr, check if its in address space
    // also need to check if < 0
    if(argptr(1, (void*)&status, sizeof(status)) < 0) {
        //Check if process does not exist
        return -1;
    }
    // call waitpid with the pid and status
    return waitpid(pid, status, 0);
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

int
sys_hello(void) {
    hello();
    return 0;
}