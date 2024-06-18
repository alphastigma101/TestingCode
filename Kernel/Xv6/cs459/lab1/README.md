# The Answers Of Power! .... Lab uno 
* What is the system call number of open?
    - Ten 
* Search the source code to find out how long a clock tick is in xv6. Read the comments. Approximately how long is one tick?
Provide evidence from the source code.
```
// ask the CLINT for a timer interrupt.
int interval = 1000000; // cycles; about 1/10th second in qemu.
*(uint64*)CLINT_MTIMECMP(id) = *(uint64*)CLINT_MTIME + interval;
```