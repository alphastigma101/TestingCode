#include <kernel/malloc.h>
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "kernel/mkvault.h"
#include "proc.h"
#include "defs.h"
#include "uproc.h"



struct cpu cpus[NCPU];
struct proc proc[NPROC];
struct proc *initproc;
shrimpy_t *Shrimpy;
stat_t *st; // Made global so kernel and user should have access to the same stat struct


void printhash(pgt_t* table) { }

int nextpid = 1;
struct spinlock pid_lock;

extern void forkret(void);
static void freeproc(struct proc *p);
extern char trampoline[]; // trampoline.S

// helps ensure that wakeups of wait()ing
// parents are not lost. helps obey the
// memory model when using p->parent.
// must be acquired before any p->lock.
struct spinlock wait_lock;

// Allocate a page for each process's kernel stack.
// Map it high in memory, followed by an invalid
// guard page.
void
proc_mapstacks(pagetable_t kpgtbl)
{
  struct proc *p;
  
  for(p = proc; p < &proc[NPROC]; p++) {
    char *pa = kalloc();
    if(pa == 0)
      panic("kalloc");
    uint64 va = KSTACK((int) (p - proc));
    kvmmap(kpgtbl, va, (uint64)pa, PGSIZE, PTE_R | PTE_W);
  }
}

// initialize the proc table.
void
procinit(void)
{
  struct proc *p;
  
  initlock(&pid_lock, "nextpid");
  initlock(&wait_lock, "wait_lock");
  for(p = proc; p < &proc[NPROC]; p++) {
      initlock(&p->lock, "proc");
      p->state = UNUSED;
      p->kstack = KSTACK((int) (p - proc));
  }
  
}

// Must be called with interrupts disabled,
// to prevent race with process being moved
// to a different CPU.
int
cpuid()
{
  int id = r_tp();
  return id;
}

// Return this CPU's cpu struct.
// Interrupts must be disabled.
struct cpu*
mycpu(void)
{
  int id = cpuid();
  struct cpu *c = &cpus[id];
  return c;
}

// Return the current struct proc *, or zero if none.
struct proc*
myproc(void)
{
  push_off();
  struct cpu *c = mycpu();
  struct proc *p = c->proc;
  pop_off();
  return p;
}

int
allocpid()
{
  int pid;
  
  acquire(&pid_lock);
  pid = nextpid;
  nextpid = nextpid + 1;
  release(&pid_lock);
  return pid;
}

// Look in the process table for an UNUSED proc.
// If found, initialize state required to run in the kernel,
// and return with p->lock held.
// If there are no free procs, or a memory allocation fails, return 0.
static struct proc*
allocproc(void) {
  struct proc *p;
  for(p = proc; p < &proc[NPROC]; p++) {
    acquire(&p->lock);
    if(p->state == UNUSED) {
      goto found;
    } else {
      release(&p->lock);
    }
  }
  return 0;

found:
  p->pid = allocpid();
  p->state = USED;
  // Allocate a trapframe page.
  if((p->trapframe = (struct trapframe *)kalloc()) == 0){
    freeproc(p);
    release(&p->lock);
    return 0;
  }
  // An empty user page table.
  p->pagetable = proc_pagetable(p);
  if(p->pagetable == 0) {
    freeproc(p);
    release(&p->lock);
    return 0;
  } 
  // Initialize fields of Shrimpy
  Shrimpy->NP = p; // These are the child processes
 
  // Set up new context to start executing at forkret,
  // which returns to user space.
  
  memset(&p->context, 0, sizeof(p->context));
  p->context.ra = (uint64)forkret;
  p->context.sp = p->kstack + PGSIZE;
  // Initialize token bucket to 64
  p->token_bucket = 64;
  // Initialize last scheduled tick to current system tick
  uint64 ctime = sys_uptime();
  p->last_scheduled_tick = ctime;
  return p;
}

// free a proc structure and the data hanging from it,
// including user pages.
// p->lock must be held.
static void
freeproc(struct proc *p)
{
  if(p->trapframe)
    kfree((void*)p->trapframe);
  p->trapframe = 0;
  if(p->pagetable)
    proc_freepagetable(p->pagetable, p->sz);
  p->pagetable = 0;
  p->sz = 0;
  p->pid = 0;
  p->parent = 0;
  p->name[0] = 0;
  p->chan = 0;
  p->killed = 0;
  p->xstate = 0;
  p->state = UNUSED;
}

// Create a user page table for a given process, with no user memory,
// but with trampoline and trapframe pages.
pagetable_t
proc_pagetable(struct proc *p)
{
  pagetable_t pagetable;

  // An empty page table.
  pagetable = uvmcreate();
  if(pagetable == 0)
    return 0;

  // map the trampoline code (for system call return)
  // at the highest user virtual address.
  // only the supervisor uses it, on the way
  // to/from user space, so not PTE_U.
  if(mappages(pagetable, TRAMPOLINE, PGSIZE,
              (uint64)trampoline, PTE_R | PTE_X) < 0){
    uvmfree(pagetable, 0);
    return 0;
  }

  // map the trapframe page just below the trampoline page, for
  // trampoline.S.
  if(mappages(pagetable, TRAPFRAME, PGSIZE,
              (uint64)(p->trapframe), PTE_R | PTE_W) < 0){
    uvmunmap(pagetable, TRAMPOLINE, 1, 0);
    uvmfree(pagetable, 0);
    return 0;
  }

  return pagetable;
}

// Free a process's page table, and free the
// physical memory it refers to.
void
proc_freepagetable(pagetable_t pagetable, uint64 sz)
{
  uvmunmap(pagetable, TRAMPOLINE, 1, 0);
  uvmunmap(pagetable, TRAPFRAME, 1, 0);
  uvmfree(pagetable, sz);
}

// a user program that calls exec("/init")
// assembled from ../user/initcode.S
// od -t xC ../user/initcode
uchar initcode[] = {
  0x17, 0x05, 0x00, 0x00, 0x13, 0x05, 0x45, 0x02,
  0x97, 0x05, 0x00, 0x00, 0x93, 0x85, 0x35, 0x02,
  0x93, 0x08, 0x70, 0x00, 0x73, 0x00, 0x00, 0x00,
  0x93, 0x08, 0x20, 0x00, 0x73, 0x00, 0x00, 0x00,
  0xef, 0xf0, 0x9f, 0xff, 0x2f, 0x69, 0x6e, 0x69,
  0x74, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00
};




/* 
  - Intended to return the physical address (pa) corresponding to a given virtual address (vaddr) 
  - if and only if the virtual address is mapped in the page table.
  - ____________________________________________________________________________________
  - Returns:
    - Returns the pa after conversion
*/
uint64 translate(pgt_t pagetable, uint64 vaddr, int level) {
  
    pte_t *pte;
    uint64 pa = 0;
    pagetable_t level_pagetable = pagetable;  // Use a temporary pagetable variable

    // Walk the page table to find the PTE for vaddr
    for (int level = 2; level > 0; level--) {
      pte = &level_pagetable[PX(level, vaddr)];
      if (!(*pte & PTE_V)) {
        return ~0; // Page not mapped
      }
      level_pagetable = (pagetable_t)PTE2PA(*pte);
    }

    // Get the PTE at the leaf level
    // Change it back to 0 after you fix the freewalk: leaf
    pte = &level_pagetable[PX(level, vaddr)];
    if (!(*pte & PTE_V)) {
      return ~0; // Page not mapped
    }
    // Calculate the physical address
    pa = PTE2PA(*pte) | (vaddr & (PGSIZE - 1));
    printf("It is mapped\n");
    printf("VALUE: %d\n", pa);
    return pa;
}

// Set up first user process.
void userinit(void) {
  if (st != (void*)0) {
    printf("Device: %d\n", st->dev);
    printf("Inode: %d\n", st->ino);
    printf("Type: %d\n", st->type);
    printf("nlink: %d\n", st->nlink);
    printf("size: %d\n", st->size);
  }
  if (Shrimpy == (void*)0) {
    table_alloc(); // allocate the table 
    // size_t is normally the bit size of a object type 
    // uint64 is the same thing as size_t but to make things more understandable I am doing this
    uint64 size =  sizeof(shrimpy_t);
    Shrimpy = (shrimpy_t*)malloc(size);
    if (Shrimpy == (void*)0) { panic("allocproc: Failed to allocate memory for shrimpy\n"); }
    /* Start referencing Shrimpy's function pointers */
    Shrimpy->map_frames = &map_frames;
    Shrimpy->map_frames_helper = &map_frames_helper; 
    Shrimpy->translate = &translate; // convert the va to pa from either user space address or kernel space
  }
  struct proc *p;
  p = allocproc();
  initproc = p;
  // allocate one user page and copy initcode's instructions
  // and data into it.
  memmove(p->pagetable, Shrimpy->NP->pagetable, sizeof(pagetable_t));
  uvmfirst(p->pagetable, initcode, sizeof(initcode));
  p->sz = PGSIZE;
  // prepare for the very first "return" from kernel to user.
  p->trapframe->epc = 0;      // user program counter
  p->trapframe->sp = PGSIZE;  // user stack pointer
  safestrcpy(p->name, "initcode", sizeof(p->name));
  p->cwd = namei("/");
  p->state = RUNNABLE;
  release(&p->lock);  
}

// Grow or shrink user memory by n bytes.
// Return 0 on success, -1 on failure.
int
growproc(int n)
{
  uint64 sz;
  struct proc *p = myproc();

  sz = p->sz;
  if(n > 0){
    if((sz = uvmalloc(p->pagetable, sz, sz + n, PTE_W)) == 0) {
      return -1;
    }
  } else if(n < 0){
    sz = uvmdealloc(p->pagetable, sz, sz + n);
  }
  p->sz = sz;
  return 0;
}

int
fork(void)
{
  int i, pid;
  struct proc *np; // This creates a new process which functions like a thread 
  struct proc *p = myproc();
  
  // Allocate process.
  if((np = allocproc()) == 0){
    return -1;
  }
  // Copy user memory from parent to child.
  if(uvmcopy(p->pagetable, np->pagetable, p->sz) < 0){
    // Copy token bucket from parent to child.
    np->ticks = 0;
    np->token_bucket = p->token_bucket;
    freeproc(np);
    release(&np->lock);
    return -1;
  }
  np->sz = p->sz;

  // copy saved user registers.
  *(np->trapframe) = *(p->trapframe);

  // Cause fork to return 0 in the child.
  np->trapframe->a0 = 0;
  
  // increment reference counts on open file descriptors.
  for(i = 0; i < NOFILE; i++)
    if(p->ofile[i])
      np->ofile[i] = filedup(p->ofile[i]);
  np->cwd = idup(p->cwd);

  safestrcpy(np->name, p->name, sizeof(p->name));
  
  // Set the number of ticks and the last scheduled tick to zero
  np->ticks = 0;
  uint64 ctime = sys_uptime();
  np->last_scheduled_tick = ctime;
  pid = np->pid;
  release(&np->lock);
  acquire(&wait_lock);
  np->parent = p;
  release(&wait_lock);
  acquire(&np->lock);
  np->state = RUNNABLE;
  release(&np->lock);
  Shrimpy->P = np; // Update parent process pointer in Shrimpy
  Shrimpy->map_frames_helper();
  return pid;
}



// Pass p's abandoned children to init.
// Caller must hold wait_lock.
void
reparent(struct proc *p)
{
  struct proc *pp;

  for(pp = proc; pp < &proc[NPROC]; pp++){
    if(pp->parent == p) {
      pp->parent = initproc;
      wakeup(initproc);
    }
  }
}

// Exit the current process.  Does not return.
// An exited process remains in the zombie state
// until its parent calls wait().
void
exit(int status)
{
  struct proc *p = myproc();

  if(p == initproc)
    panic("init exiting");

  // Close all open files.
  for(int fd = 0; fd < NOFILE; fd++){
    if(p->ofile[fd]){
      struct file *f = p->ofile[fd];
      fileclose(f);
      p->ofile[fd] = 0;
    }
  }

  begin_op();
  iput(p->cwd);
  end_op();
  p->cwd = 0;

  acquire(&wait_lock);
  // Give any children to init.
  reparent(p);

  // Parent might be sleeping in wait().
  wakeup(p->parent);
  acquire(&p->lock);

  p->xstate = status;
  p->state = ZOMBIE;

  release(&wait_lock);

  // Jump into the scheduler, never to return.
  sched();
  panic("zombie exit");
}

// Wait for a child process to exit and return its pid.
// Return -1 if this process has no children.
int
wait(uint64 addr)
{
  struct proc *pp;
  int havekids, pid;
  struct proc *p = myproc();

  acquire(&wait_lock);
  for(;;){
    // Scan through table looking for exited children.
    havekids = 0;
    for(pp = proc; pp < &proc[NPROC]; pp++){
      if(pp->parent == p){
        // make sure the child isn't still in exit() or swtch().
        acquire(&pp->lock);

        havekids = 1;
        if(pp->state == ZOMBIE){
          // Found one.
          pid = pp->pid;
          if(addr != 0 && copyout(p->pagetable, addr, (char *)&pp->xstate,
                                  sizeof(pp->xstate)) < 0) {
            release(&pp->lock);
            release(&wait_lock);
            return -1;
          }
          freeproc(pp);
          release(&pp->lock);
          release(&wait_lock);
          return pid;
        }
        release(&pp->lock);
      }
    }

    // No point waiting if we don't have any children.
    if(!havekids || killed(p)){
      release(&wait_lock);
      return -1;
    }
    // Wait for a child to exit.
    sleep(p, &wait_lock);  //DOC: wait-sleep
  }
}

void FATCAT_SCHED(struct proc *p, uint64 current_tick) {
  int tick_diff =  current_tick - p->last_scheduled_tick; // Calculate the time difference
  if ( p->token_bucket < 65 && tick_diff > 0) {
    for (int i = 0; i < tick_diff; i++) {
      // Perform actions here when i is a multiple of 10
      if ((i + 1) % 10 == 0) {
        // i + 1 because i starts from 0, but we want to check multiples of 10
        p->token_bucket++; // Add a token to the bucket
      }
    }    
  }
  p->last_scheduled_tick = current_tick; // Update last token tick
  p->avialable_tokens = p->token_bucket; // update the tokens that are used
  
  return;
}

// Per-CPU process scheduler.
// Each CPU calls scheduler() after setting itself up.
// Scheduler never returns.  It loops, doing:
//  - choose a process to run.
//  - swtch to start running that process.
//  - eventually that process transfers control
//    via swtch back to the scheduler.
void
scheduler(void) {
  struct proc *p;
  struct cpu *c = mycpu();
  uint64 current_tick = sys_uptime();
  c->proc = 0;
  
  for(;;){
    // Avoid deadlock by ensuring that devices can interrupt.
    intr_on();
    for(p = proc; p < &proc[NPROC]; p++) {
      acquire(&p->lock);
      if(p->state == RUNNABLE) {
        if(FATCAT_SCHEDULER == 1) { 
          p->state = RUNNING;
          c->proc = p;    
          swtch(&c->context, &p->context);
          FATCAT_SCHED(p, current_tick);    
        }
        else {
          // Switch to chosen process.  It is the process's job
          // to release its lock and then reacquire it
          // before jumping back to us.
          p->state = RUNNING;
          c->proc = p;
          swtch(&c->context, &p->context);
          
        }
        c->proc = 0;
      }
      release(&p->lock);
    }
  }
  
}

// Switch to scheduler.  Must hold only p->lock
// and have changed proc->state. Saves and restores
// intena because intena is a property of this
// kernel thread, not this CPU. It should
// be proc->intena and proc->noff, but that would
// break in the few places where a lock is held but
// there's no process.
void
sched(void) {
  int intena;
  struct proc *p = myproc();
  uint64 ctick;

  if(!holding(&p->lock))
    panic("sched p->lock");
  if(mycpu()->noff != 1)
    panic("sched locks");
  if(p->state == RUNNING)
    panic("sched running");
  if(intr_get())
    panic("sched interruptible");
  // Get current tick count
  ctick = sys_uptime();

  // Compare current tick with last scheduled tick
  // Get the previous scheduled tick 
  // NOTE: Works decently with CPU = 3 in Makefile
  int tick_diff = ctick - p->last_scheduled_tick;
  //printf("%s\t%d\t", "Printing the difference!", tick_diff);
  if (tick_diff > 0 && p->state == RUNNABLE) {
    //printf("%s\t\%d\t", "Printing Ticks!", p->ticks);
    // Remove a token for each tick that has passed
    for (int i = 0; i < tick_diff; i++) {
      // Check if the process is still runnable
      if (p->state == RUNNABLE && p->token_bucket > 0) {
        // Decrement the token bucket
        p->token_bucket--;
      }
    }
    // Update the ticks for each processor
    p->ticks += tick_diff;
  }
  else if (p->token_bucket < 1 && p->state == SLEEPING) {
    p->state = RUNNABLE; 
    // Get the previous ticks
    p->pticks = p->ticks;
    p->time = ctick;
    intena = mycpu()->intena;
    swtch(&p->context, &mycpu()->context);
    mycpu()->intena = intena;
  }
  p->time = ctick;
  // Update the available tokens
  p->avialable_tokens = p->token_bucket;
  
  // Update the last scheduled tick to the current tick
  p->last_scheduled_tick = ctick;
  intena = mycpu()->intena;
  swtch(&p->context, &mycpu()->context);
  mycpu()->intena = intena;
  
}

// Give up the CPU for one scheduling round.
void
yield(void)
{
  struct proc *p = myproc();
  acquire(&p->lock);
  p->state = RUNNABLE;
  sched();
  release(&p->lock);
}

// A fork child's very first scheduling by scheduler()
// will swtch to forkret.
void
forkret(void)
{
  static int first = 1;

  // Still holding p->lock from scheduler.
  release(&myproc()->lock);

  if (first) {
    // File system initialization must be run in the context of a
    // regular process (e.g., because it calls sleep), and thus cannot
    // be run from main().
    first = 0;
    fsinit(ROOTDEV);
  }

  usertrapret();
}

// Atomically release lock and sleep on chan.
// Reacquires lock when awakened.
void
sleep(void *chan, struct spinlock *lk)
{
  struct proc *p = myproc();
  
  // Must acquire p->lock in order to
  // change p->state and then call sched.
  // Once we hold p->lock, we can be
  // guaranteed that we won't miss any wakeup
  // (wakeup locks p->lock),
  // so it's okay to release lk.

  acquire(&p->lock);  //DOC: sleeplock1
  release(lk);

  // Go to sleep.
  p->chan = chan;
  p->state = SLEEPING;

  sched();

  // Tidy up.
  p->chan = 0;

  // Reacquire original lock.
  release(&p->lock);
  acquire(lk);
}

// Wake up all processes sleeping on chan.
// Must be called without any p->lock.
void
wakeup(void *chan)
{
  struct proc *p;

  for(p = proc; p < &proc[NPROC]; p++) {
    if(p != myproc()){
      acquire(&p->lock);
      if(p->state == SLEEPING && p->chan == chan) {
        p->state = RUNNABLE;
      }
      release(&p->lock);
    }
  }
}

// Kill the process with the given pid.
// The victim won't exit until it tries to return
// to user space (see usertrap() in trap.c).
int
kill(int pid)
{
  struct proc *p;

  for(p = proc; p < &proc[NPROC]; p++){
    acquire(&p->lock);
    if(p->pid == pid){
      p->killed = 1;
      if(p->state == SLEEPING) {
        // Wake process from sleep().
        p->state = RUNNABLE;
      }
      release(&p->lock);
      return 0;
    }
    release(&p->lock);
  }
  return -1;
}

void
setkilled(struct proc *p)
{
  acquire(&p->lock);
  p->killed = 1;
  release(&p->lock);
}

int
killed(struct proc *p)
{
  int k;
  
  acquire(&p->lock);
  k = p->killed;
  release(&p->lock);
  return k;
}

// Copy to either a user address, or kernel address,
// depending on usr_dst.
// Returns 0 on success, -1 on error.
int
either_copyout(int user_dst, uint64 dst, void *src, uint64 len)
{
  struct proc *p = myproc();
  if(user_dst){
    return copyout(p->pagetable, dst, src, len);
  } else {
    memmove((char *)dst, src, len);
    return 0;
  }
}

// Copy from either a user address, or kernel address,
// depending on usr_src.
// Returns 0 on success, -1 on error.
int
either_copyin(void *dst, int user_src, uint64 src, uint64 len)
{
  struct proc *p = myproc();
  if(user_src){
    return copyin(p->pagetable, dst, src, len);
  } else {
    memmove(dst, (char*)src, len);
    return 0;
  }
}

// Print a process listing to console.  For debugging.
// Runs when user types ^P on console.
// No lock to avoid wedging a stuck machine further.
void
procdump(void) {
  static char *states[] = {
  [UNUSED]    "unused",
  [USED]      "used",
  [SLEEPING]  "sleep ",
  [RUNNABLE]  "runble",
  [RUNNING]   "run   ",
  [ZOMBIE]    "zombie"
  };
  struct proc *p;
  char *state;

  printf("\n");
  for(p = proc; p < &proc[NPROC]; p++){
    if(p->state == UNUSED) {
      continue;
    }
    if(p->state >= 0 && p->state < NELEM(states) && states[p->state])
      state = states[p->state];
    else
      state = "???";
    printf("%d %s %s", p->pid, state, p->name);
    printf("\n");
  }
}
pte_t *pte;

// Helper function to unmap a page if it exists
void unmap(pagetable_t pagetable, uint64 va, int level) {
  pte = walk(pagetable, va, 1); // 0 to not create new pages
  if (pte && (*pte & PTE_V)) {
    // Ensure it is not a leaf node
    if ((*pte & (PTE_R | PTE_W | PTE_X)) == 0) {
      uvmunmap(pagetable, va, 1, 0);
      printf("Unmapped va %p in pagetable %p\n", (void*)va, (void*)pagetable);
    } else {
      printf("Skipping unmapping leaf va %p in pagetable %p\n", (void*)va, (void*)pagetable);
    }
  }
}

static uint64 *fry_page, *shrimp_page;
static uint64 *heap_page, *stack_page, *guard_page, *data_page, *context_page;
void map_frames()  {
  // Unmap existing mappings in the child's pagetable
  unmap(Shrimpy->NP->pagetable, FRY, 0);
  unmap(Shrimpy->NP->pagetable, SHRIMP, 0);
  unmap(Shrimpy->NP->pagetable, HEAP, 0);
  unmap(Shrimpy->NP->pagetable, STACK, 0);
  unmap(Shrimpy->NP->pagetable, GUARD_PAGE, 0);
  unmap(Shrimpy->NP->pagetable, DATA, 0);
  unmap(Shrimpy->NP->pagetable, TEXT, 0);
  // Allocate and map the fry page
  if (fry_page == (void*)0) {
    fry_page = (uint64*)kalloc();
    Shrimpy->pas[0] = fry_page;
  }
  if (fry_page == (void*)0) { panic("map_frames: failed to allocate fry page"); }
  if (mappages(Shrimpy->P->pagetable, Shrimpy->vas[0], PGSIZE, (uint64)*fry_page, PTE_R | PTE_W | PTE_X | PTE_U | PTE_V) != 0) {
    kfree(fry_page);
    panic("map_frames: failed to map fry page");
  }
  // Allocate and map the shrimp page
  if (shrimp_page == (void*)0) {
    shrimp_page = (uint64*)kalloc();
    Shrimpy->pas[1] = shrimp_page;
  }
  if (shrimp_page == (void*)0) { panic("map_frames: failed to allocate shrimp page"); }
  if (mappages(Shrimpy->P->pagetable, Shrimpy->vas[1], PGSIZE, (uint64)*shrimp_page, PTE_R | PTE_W | PTE_X | PTE_U | PTE_V) != 0) {
    kfree(shrimp_page);
    panic("map_frames: failed to map shrimp page");
  }
  // Allocate memory for the heap page frame 
  if (heap_page == (void*)0) {
    heap_page = (uint64*)kalloc();
    Shrimpy->pas[2] = heap_page;
  }
  if (heap_page == (void*)0) { panic("Failed to allocate memory for heap_page frame...\n"); }
  if (mappages(Shrimpy->P->pagetable, Shrimpy->vas[2], PGSIZE, (uint64)*heap_page, PTE_R | PTE_W | PTE_X | PTE_U) != 0) {
    kfree(heap_page);
    panic("map_frames: failed to map heap page(uint64)HEAP");
  }
  // Create the stack pages
  if (stack_page == (void*)0) {
    stack_page = (uint64*)kalloc();
    Shrimpy->pas[3] = stack_page;
  }
  if (stack_page == (void*)0) { panic("map_frames: failed to allocate context page"); }
  if (mappages(Shrimpy->P->pagetable, Shrimpy->vas[3], PGSIZE, (uint64)*stack_page, PTE_R | PTE_W | PTE_X | PTE_U) != 0) {
    kfree(stack_page);
    panic("map_frames: failed to map context page");
  }
  // Create the guard pages
  if (guard_page == (void*)0) {
    guard_page = (uint64*)kalloc();
    Shrimpy->pas[4] = guard_page;
  }
  if (guard_page == (void*)0) { panic("map_frames: failed to allocate context page"); }
  if (mappages(Shrimpy->P->pagetable, Shrimpy->vas[4], PGSIZE, (uint64)*guard_page, PTE_R | PTE_W | PTE_X | PTE_U) != 0) {
    kfree(guard_page);
    panic("map_frames: failed to map context page");
  }
  // Create the data pages
  if (data_page == (void*)0) {
    data_page = (uint64*)kalloc();
    Shrimpy->pas[5] = data_page;
  }
  if (data_page == (void*)0)
    panic("map_frames: failed to allocate context page");
  if (mappages(Shrimpy->P->pagetable, Shrimpy->vas[5], PGSIZE, (uint64)*data_page, PTE_R | PTE_W | PTE_X | PTE_U) != 0) {
    kfree(data_page);
    panic("map_frames: failed to map context page");
  }
  // Create the pages for context text
  if (context_page == (void*)0) {
    context_page = (uint64*)kalloc();
    Shrimpy->pas[6] = context_page;
  }
  if (context_page == (void*)0) {
    panic("map_frames: failed to allocate context page");
  }
  if (mappages(Shrimpy->P->pagetable, Shrimpy->vas[6], PGSIZE, (uint64)*context_page, PTE_R | PTE_W | PTE_X | PTE_U) != 0) {
    kfree(context_page);
    panic("map_frames: failed to map context page");
  }
  // Perform the memmove operation to copy data
  memmove(Shrimpy->NP->pagetable, Shrimpy->P->pagetable, sizeof(pagetable_t));
  if (SHRIMPY == 0) {
    printf("Testing from kernel space at level zero....\n");
    for (int i = 0; i < 7; i++) {
      Shrimpy->translate(Shrimpy->P->pagetable, Shrimpy->vas[i], 0);
    }
  }
  return;
} 


void map_frames_helper() {
   /* 
    - One page is shared with the parent of P. This page is mapped at the shrimp virtual address, and is called the shrimp page for P. If the process does not have a parent, then behavior upon accessing this page is undefined.
    - One page is shared with the children of P. All children use the same physical page. This page is mapped at the fry virtual address, and is called the fry page for P.
    - These are single pages. It is not possible to grow or shrink the amount of shared memory.
    - When a process P forks and creates a child process NP, PHY(P, fry) is mapped to PHY(NP, shrimp) in the page table for NP.
  */
  Shrimpy->vas[0] = (uint64)FRY;
  Shrimpy->vas[1] = (uint64)SHRIMP;
  Shrimpy->vas[2] = (uint64)HEAP;
  Shrimpy->vas[3] = (uint64)STACK;
  Shrimpy->vas[4] = (uint64)GUARD_PAGE;
  Shrimpy->vas[5] = (uint64)DATA;
  Shrimpy->vas[6] = (uint64)TEXT;
 return Shrimpy->map_frames();
} 

// List the processes that the user is running when the user issues the ps command
// Will copy the processes obects that are running, (the proc struct ) to the user proc struct (uproc)
// List the processes that the user is running when the user issues the ps command
// Will copy the processes obects that are running, (the proc struct ) to the user proc struct (uproc)
int pslist(uint64 utable) {
  static char *states[] = {
    [UNUSED]    "unused",
    [USED]      "used",
    [SLEEPING]  "sleep ",
    [RUNNABLE]  "runble",
    [RUNNING]   "run ",
    [ZOMBIE]    "zombie"
  };

  struct proc *p;
  struct uproc u;  // Temporary user process structure
  struct proc *current_process = myproc(); // Get the current process
  int count = 0;
  // Print header
  printf("PID\tSTATE\tNAME\tTICKS\tAVIAL\n");
  // Iterate through the proc struct 
  for(p = proc; p < &proc[NPROC]; p++) {
    if(p->state == UNUSED) { continue; }
    u.pid = p->pid; // Connect user process with user process
    u.avialable_tokens = p->avialable_tokens; // Connect the processor's tokens to user processor
    u.pticks = p->pticks; // set the previous ticks with the user previous ticks
    // To get the correct calculations, all you need to do is: 
    // Get the current ticks + avialable tokens 
    // And minus it by the previous ticks
    int tick_diff = p->time + p->avialable_tokens - p->last_scheduled_tick;
    if (tick_diff > 0) {
      u.ticks = 0;
      for (int i = 0; i < tick_diff; i++) {
        u.ticks++; // Connect the ticks with the user processor
        if (u.avialable_tokens > 0) { u.avialable_tokens--; }
        else if (u.avialable_tokens < 1) {
          // Make the process sleep instead of letting it execute 
          p->state = SLEEPING;
          u.ticks = p->pticks;
          u.avialable_tokens = 64;
          p->avialable_tokens = 64;
        }
      }
    }
    safestrcpy(u.state, states[p->state], sizeof(u.state));  // Copy state string
    safestrcpy(u.name, p->name, sizeof(u.name));  // Copy process name
    // Print process information
    printf("%d\t%s\t%s\t%d\t%d\n", u.pid, u.state, u.name, u.ticks, u.avialable_tokens);
    // Copy data to user space using copyout
    if(copyout(current_process->pagetable, utable, (char*)&u, sizeof(struct uproc)) < 0) {
      return -1;  // Failed to copy out
    }
    utable += sizeof(struct uproc);  // Move the user table pointer
    count++;  // Increment the process counter
  }
  return count;  // Return the number of processes copied
}

int shrimpy(shrimpy_t* ushrimp) {
  ushrimp->translate = &translate;
  printf("Translating vas to match with pas...\n");
  pagetable_t *table = &ushrimp->P->pagetable;
  for (int i = 0; i < 7; i++) {
    ushrimp->translate(*table, ushrimp->vas[i], 1);
    printf("=============================\n");
    printf("Finished Testing Shrimpy... Have a nice day!\n");
    exit(0);
  }
  return 0;
}

int mkvault(uint64 umkvault) {
  //char* path = (char*)umkvault;
  // Check if the directory is empty
  if (isdirempty(mkv->inode) == -1) {
    printf("Directory is not empty, cannot mark as vault\n");
    return -1; // Return an error code if the directory is not empty
  }
  else {
    // Allocate memory for stat structure if needed
    if (st == (void*)0) {
      printf("Allocating memory for st (stat)..\n");
      st = (stat_t*)kalloc();  // Allocate memory for stat structure in kernel space
      if (st == (void*)0) {
        panic("Failed to allocate memory for stat");
      }
    }
    // My system call sets the is_vault to one when the folder does not exist
    mkv->st = st;
    mkv->inode->is_vault = 1;
    mkv->st->is_vault = 1;
    mkv->inode->vault_id = mkv->inode->inum;
    mkv->st->vault_id = mkv->inode->inum;
    stati(mkv->inode, mkv->st);
  }
  mkv->m.ied = &isdirempty; // Link it
  return 0;
}

int openvault(uint64 uopenvault) {
  char* path = (char*)uopenvault;
  if (mkv->st->is_vault && !mkv->st->vault_open) {
    // Validate key (for now, just set vault_open)
    // In a real scenario, validate the key properly
    mkv->st->vault_open = 1; // Mark vault as open
    // Update the inode
    iupdate(mkv->inode);
    printf("Vault %s is now open\n", path);
  } 
  else { 
    printf("Directory is not a vault or is already open\n"); 
    return -1;
  }
  return 0;
}

int closevault(uint64 uclosevault) {
  char* path = (char*)uclosevault;
  if (mkv->st->is_vault && mkv->st->vault_open) {
    mkv->st->vault_open = 0; // Mark vault as closed
    // Update the inode
    iupdate(mkv->inode);
    printf("Vault %s is now closed\n", path);
  } 
  else { 
    printf("Directory is not a vault or is already closed\n"); 
    return -1;
  }
  return 0;
}