#include "kernel/stat.h"
#include "kernel/spinlock.h"
#include "kernel/sleeplock.h"
#include "kernel/file.h"
#include "kernel/fcntl.h"

typedef struct mkvault {
    int inode_num;
    int key;
    int is_valid;
    union {
        int (*ied)(struct inode*); // function sig to check and see if a directory is empty
        void (*mav)(); // mark the directory as a vault 
    } m; // methods
    inode_t *inode; 
    stat_t *st;
    Align x;

} mkvault_t;
int snprintf(char * restrict dest, size_t n, const char * restrict format, ... );
extern mkvault_t *mkv; // make mkv global so the user and kernel can use the same object 






