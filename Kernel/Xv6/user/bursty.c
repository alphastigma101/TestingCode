#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

unsigned long randstate;
unsigned int rand() { 
    randstate = randstate * 1664525 + 1013904223;
    return (randstate % 300) + 150;
}

int main() {
   randstate = uptime();
   for (;;) {
    uint64 start = uptime();
    for(int i = 0; i < 1000000000; ++i) {}
       uint64 end = uptime();

       printf("bursty: %d, %d\n", start, end - start);

       unsigned int s = rand();
       sleep(s);
    }
    return 0;
}