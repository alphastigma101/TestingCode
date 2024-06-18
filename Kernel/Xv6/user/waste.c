#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    for (;;) {      
        uint64 start = uptime();
        for(int i = 0; i < 1000000000; ++i) {}
        uint64 end = uptime();
        printf("waste: %d, %d\n", start, end - start);
    }
    return 0;
}