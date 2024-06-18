#include <kernel/shrimpy.h>
struct uproc;

// system calls
int fork(void);
int exit(int) __attribute__((noreturn));
int wait(int*);
int pipe(int*);
int write(int, const void*, int); // This is the system call I used. It attempts to write nbyte bytes from the buffer pointed to by buf to the file associated with the open file descriptor. Source: https://pubs.opengroup.org/onlinepubs/009696699/functions/write.html
int read(int, void*, int); // This is the system call I used. It reads in the buffer stack and counts up to it. Source: https://man7.org/linux/man-pages/man2/read.2.html
int close(int); // This is the system call I used. It closes the file which literally deallocates the buffer. 
int kill(int);
int exec(const char*, char**);
int open(const char*, int); // This is the call I used. It establishes the connection between a file and a file descriptor. Source: https://pubs.opengroup.org/onlinepubs/7908799/xsh/open.html
int mknod(const char*, short, short);
int unlink(const char*);
int fstat(int fd, struct stat*);
int link(const char*, const char*);
int mkdir(const char*);
int chdir(const char*);
int dup(int);
int getpid(void);
char* sbrk(int);
int sleep(int); // This allows pids to go to sleep. Source: Bite me
int uptime(void);
void cp(const char*, const char*); // Might have to turn these into ints instead of voids
void touch(const char*);
void bestfit(unsigned int sizes[], int n);
int pslist(struct uproc*);
int shrimpy(shrimpy_t *ushrimp);
int mkvault(uint64 umkvault);
int openvault(uint64 uopenvault);
int closevault(uint64 uclosevault);




// ulib.c
int stat(const char*, struct stat*);
char* strcpy(char*, const char*);
void *memmove(void*, const void*, int);
char* strchr(const char*, char c);
int strcmp(const char*, const char*);
void fprintf(int, const char*, ...);
void printf(const char*, ...);
char* gets(char*, int max);
uint strlen(const char*);
void* memset(void*, int, uint);
void* malloc(uint);
void free(void*);
int atoi(const char*);
int memcmp(const void *, const void *, uint);
void *memcpy(void *, const void *, uint);
