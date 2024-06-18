# Clean up your space by compressing folders 
* This is the command I used:
```
    tar cf - directory_name/ | zstd > compressed_file_name. tar
```
# Uncompresson:
```
    zstd -d /path/to/archive.tar.zst -o /path/to/output_folder
```
# To finish this assignment you will need to modify at least the following files in xv6:
```
Makefile

user/ps.c

user/user.h

user/usys.pl

kernel/uproc.h

kernel/syscall.h

kernel/syscall.c

kernel/defs.h

kernel/proc.c

kernel/sysproc.c
```
# Hello, kernel
* In this section will be our first foray into writing kernel code! We are going to implement a utility ps that will list the processes of a system. On a unix-like system such as macOS or Linux, you can type ps and you will see a list of running processes.

```
➜  ~ ps
```
```
    PID TTY      	TIME CMD
    1492 pts/4	00:00:00 zsh
    1521 pts/4	00:00:00 ps
```
* We are going to implement something similar for xv6, but with only a tiny fraction of the functionality that the real ps utility has.

* Here is the output of my xv6 ps utility:
```
    $ ps
```
```
    PID 	STATE   NAME
    1   	sleep   init
    2   	sleep   sh
    3   	run 	ps
```
# Part 3. Write empty ps program
* Create a new userspace program ps and provide a main function. After this step the ps program should run in xv6, but it does not do anything useful.

# Part 4. Add uproc struct
* The xv6 proc struct is defined in kernel/proc.h. This structure contains more information than we need (or that the kernel would like a userspace program to have). Therefore we are going to create a uproc struct that holds only a the information we need to implement ps. Our ps utility needs three pieces of information for each process: the process ID, the name of the running process, and the state that the process is in.

* Create a file kernel/uproc.h and put the following in it:
```
struct uproc {
   int pid;
   char name[16];
   char state[7];
};
```
# Part 5. Add empty pslist system call
* Create a new system call named pslist. This system call should have the following signature in user.h. The pointer parameter is meant to be an array of uproc structs; this is an out parameter that will be populated by the kernel. The return value of the system call is the number of processes in the array.
```
    struct uproc;

    // ...

    int pslist(struct uproc*);
```
* Modify usys.pl to create the glue functions for this system call, and modify kernel/syscall.h to assign your new system call a syscall number.

* In kernel/syscall.c the signature of your system call definition should be
```
    extern uint64 sys_pslist(void);
```
* xv6 uses the uint64 type to represent memory addresses that are managed by the kernel.

* Add your system call to the syscalls array.

* **Note:** that even though the return type is an unsigned integer, you will be able to return a negative number. I recommend returning -1 if an error occurs. After this step, your system call should exist and be callable, but it does not yet do anything. Note that this function does not take any parameters. You will need to use argaddr in step 5 to retrieve the first and only parameter of the pslist system call as a pointer.

* Create a stub definition of sys_pslist in kernel/sysproc.c.

# Part 6. Test system call with ps
* Add a call to pslist from ps to make sure that everything compiles and that you can execute your system call from a user application.

* At this point you might be wondering what the size of our array of uproc structs should be. The value NPROC in kernel/param.h defines the maximum number of processes in an xv6 system.

* After completing this step, you should be able to pass an array of uproc structs to the pslist system call in ps.c, and the system call should execute. We have not yet added any functionality to the system call.

# Part 7. Implement pslist system call
* We are going to create an internal kernel function that reads the list of processes. Modify kernel/defs.h to add the following function signature.
```
    int pslist(uint64 utable);
```
* Use argaddr to retrieve the first and only parameter of the pslist system call as a pointer. In the xv6 kernel it is common to use the uint64 type to store pointer values.

* The xv6 process list is in kernel/proc.c. Create a new function in proc.c that will loop over the process list. You probably want this function to take a single uint64 parameter. Look at and borrow from the procdump function to complete this step. This function will be called from sys_pslist in sysproc.c. Skip processes in the UNUSED state.

* The memory address of the array of uproc structs originated in ps.c and has been threaded all the way through to proc.c. This means that it is a user address. Do not write to user addresses directly. Do not write to user addresses directly. The kernel will panic. We want a calm kernel.

* Instead, you need to use copyout. Create a uproc struct with the relevant data, and then use copyout to copy data to the user process. The first parameter to copyout is the pagetable of the calling process, you can retrieve the calling process with myproc(). Look for examples in xv6. The sys_pipe function is one example that you can look at.

* In C, the size of a struct is not necessarily the same as the sum of the sizes of its fields because the C compiler is allowed to add padding between fields to ensure proper alignment in memory. Proper alignment is important for efficient memory access on many modern CPUs. Accessing data off alignment may require extra memory fetches or complex operations, resulting in slower performance.

* The sizeof operator in C returns the size of a variable or data type in bytes. When used on a struct, sizeof returns the total size of the struct, including any padding that the C compiler may have added between fields to ensure proper alignment. For example, consider the following struct:
```
struct Example {
    int a;
    char b;
    double c;
};
```
* To determine the size of this struct, including any padding added by the compiler, you can use sizeof(struct Example). This will return the total size of the struct, which may be larger than the sum of the sizes of its individual fields due to added padding.

* You will want the copyout code to look something like this:
```
    struct proc *myp = myproc();
    copyout(myp->pagetable, POINTER_ARITH,  (char *) COPYING_FROM, HOW_MUCH_TO_COPY);
```
# Part 8. Print out the process list
* The pointer to uproc structs that ps.c passes to the pslist system call should now be populated with real values!

* Print them to stdout in the order “%d %s %s”, pid, state, name. Match my output below. The ampersand after sleep puts the process in the background. This is a useful trick in many shells.

* Use the following strings to represent process states: “unused”, “sleep ”, “runble”, “run ”, “zombie”.
```
$ ps
```
```
PID 	STATE   NAME
1   	sleep   init
2   	sleep   sh
3   	run 	ps
```
$ sleep 100 &
$ ps
```
```
PID 	STATE   NAME
1   	sleep   init
2   	sleep   sh
6   	run 	ps
5   	sleep   sleep
```
# Submission
* Push to the main branch of your GitHub repository.
