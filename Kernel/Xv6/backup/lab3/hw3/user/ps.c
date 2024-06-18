#include "kernel/param.h"  
#include "kernel/uproc.h"
#include "user/user.h" 

struct uproc uproc_array[NPROC]; // this is a sys call array which is a pointer to an array
int main(int argc, char *argv[]) {
    int num_procs = pslist(uproc_array);
    if (num_procs < 0) {
        // Handle error
        printf("pslist call failed\n");
        exit(1);
    }
    uproc_array->avialable_tokens = uproc_array->avialable_tokens % 64;
    // For now, just indicate success
    printf("number of processes: %d\n", num_procs);
    return 0;
}
