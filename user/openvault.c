#include "kernel/mkvault.h"
#include "user/user.h"
#include <stdarg.h>
stat_t *st;
mkvault_t *mkv;
hash_t *ht; // Use the hash table you created 
int main(int argc, char *argv[]) {
    int fd; // declare your int variable 
    if (argc != 2) {
        fprintf(2, "Usage: openvault name_of_directory \n");
        exit(1);
    }
    if ((fd = open(argv[1], O_RDONLY)) < 0) {
        fprintf(2, "openvault: cannot open %s\n", argv[1]);
        exit(1);
    }
    if (fstat(fd, st) < 0) {
        fprintf(2, "openvault: cannot stat %s\n", argv[1]);
        close(fd);
        exit(1);
    }
    if ( openvault((uint64)argv[1]) == -1 ) {

    }
    return 0;
}