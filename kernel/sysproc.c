#include "kernel/mkvault.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "proc.h"
mkvault_t *mkv;
// Here is the defintion of sys_pslist 
uint64 sys_pslist(void) {
  uint64 utable;
  argaddr(0, &utable);
  return pslist(utable);
}
uint64 sys_shrimpy(void) {
  uint64 ushrimp;
  // Retrieve the user-space destination address
  argaddr(0, &ushrimp);
  // Copy the Shrimpy structure from kernel space to user space
  if (copyout(Shrimpy->P->pagetable, ushrimp, (char *)Shrimpy, sizeof(pagetable_t)) < 0) {
    return -1;
  }
  return (uint64)shrimpy(Shrimpy);
}

uint64 sys_mkvault(void) {
  char buff[100]; 
  int max = sizeof(buff); // Get the max size that should be read to the kernel 

  // Use argstr to read the path into the kernel
  int n = argstr(0, buff, max);
  if (n == -1) {
    return -1;
  }

  // Print the path read into the kernel
  printf("Path read into the kernel: %s\n", buff);

  if (mkv == (void*)0) {
    // Allocate memory only for mkv struct and exclude allocating memory for inode_t
    printf("allocating memory for mkv (mkvault)....\n");
    mkv = (mkvault_t*)kalloc();
    if (mkv == (void*)0) {
      panic("Failed to allocate memory for mkv");
    }
  }
  // Use namei to lookup the inode associated with the path
  mkv->inode = namei(buff);
  // Check if namei was successful
  if (mkv->inode == 0) {
    printf("Failed to lookup inode for path: %s\n", buff);

    // Start a file system transaction
    begin_op();
    mkv->inode = ialloc(ROOTDEV, T_DIR); // Allocate inode on root device as a directory
    if (mkv->inode == 0) {
      end_op();
      panic("Failed to allocate inode for mkvault");
    }
    // End the file system transaction
    end_op();
    return mkvault((uint64)buff);
  } 
  else {
    // The folder already exists, so we need to get the inode's values
    printf("Inode lookup successful!\n");

    // Allocate memory for stat structure if needed
    if (st == (void*)0) {
      printf("Allocating memory for st (stat)..\n");
      st = (stat_t*)kalloc();  // Allocate memory for stat structure in kernel space
      if (st == (void*)0) {
        panic("Failed to allocate memory for stat");
      }
    }
    mkv->st = st; // Link it 
    printf("mkv->inode->dev: %d\n", mkv->inode->dev);
    printf("mkv->inode->type: %d\n", mkv->inode->type);
    printf("mkv->inode->nlink: %d\n", mkv->inode->nlink);
    printf("mkv->inode->size: %d\n", mkv->inode->size);
    // Fill the stat structure with inode values
    stati(mkv->inode, mkv->st);
    printf("mkv->st->dev: %d\n", mkv->st->dev);
    printf("mkv->st->type: %d\n", mkv->st->type);
    printf("mkv->st->nlink: %d\n", mkv->st->nlink);
    printf("mkv->st->size: %d\n", mkv->st->size);
  }
  // Return back to mkvault function
  return mkvault((uint64)buff);
}

uint64 sys_openvault(void) {
  char buff[100]; 
  int max = sizeof(buff); // Get the max size that should be read to the kernel 
  // Use argstr to read the path into the kernel
  int n = argstr(0, buff, max);
  if (n == -1) {
    printf("Failed to copy the directory to kernel..\n");
    return -1;
  }
  return openvault((uint64)buff);
}

uint64 sys_closevault(void) {
  char buff[100]; 
  int max = sizeof(buff); // Get the max size that should be read to the kernel 
  // Use argstr to read the path into the kernel
  int n = argstr(0, buff, max);
  if (n == -1) {
    printf("Failed to copy the directory to kernel..\n");
    return -1;
  }
  return closevault((uint64)buff);
}

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
