//#include "kernel/types.h"
//#include "kernel/stat.h"
#include <user/user.h>
int main(int argc, char *argv[]) {
   sleep(atoi(argv[1]));
   exit(0);

}
/*
int sleep(int time) {
    // Implement what sleep should do here
    // You need examples of what it should really look like
    // Calculate the target tick count based on the current system tick count
    int target_ticks = 0; 
    while (target_ticks < time) {
        target_ticks++;
    }
    // Need to get the ratio and assign it to u.ticks 
    // Using 


    return 0; // sleep was successful
}
*/
/*
int wait(int* time) {

    return 0;
}
*/
