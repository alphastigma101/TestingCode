#include "kernel/mkvault.h"
#include "user/user.h"
#include <stdarg.h>
stat_t *st;
mkvault_t *mkv;
int main(int argc, char *argv[]) {
    int fd;
    if (argc != 2) {
        fprintf(2, "Usage: closevault name_of_directory \n");
        exit(1);
    }
    if ((fd = open(argv[1], O_RDONLY)) < 0) {
        fprintf(2, "closevault: cannot open %s\n", argv[1]);
        exit(1);
    }
    if (fstat(fd, st) < 0) {
        fprintf(2, "closevault: cannot stat %s\n", argv[1]);
        close(fd);
        exit(1); 
    }
    if (closevault((uint64)argv[1]) == -1 ) {
        // Need to add code here 
        // Probably do it later tonight or tomorrow 

    }
    close(fd);
    return 0;
}