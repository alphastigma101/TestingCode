#include "kernel/mkvault.h"
#include "user/user.h"
stat_t *st;
mkvault_t *mkv;
// Driver code definition
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(2, "Usage: stat name_of_file with any type of extension\n");
        exit(1);
    }
    if (stat(argv[1], st) == -1) {
        printf("Error: Something happend\n");
        exit(-1);
    }
    else {
        if (mkv != (void*)0) {
            printf("Linking mkv (mkvault_t) with st (stat_t)\n");
            mkv->st = st; // Link it 
        }
        printf("Device: %d\n", st->dev);
        printf("Inode: %d\n", st->ino);
        printf("Type: %d\n", st->type);
        printf("nlink: %d\n", st->nlink);
        printf("size: %d\n", st->size);
        printf("is_vault: %d\n", st->is_vault);
        printf("vault_id: %d\n", st->vault_id);
        exit(0);
    }
    return -1;
}