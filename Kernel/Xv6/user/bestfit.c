#include "user/user.h"

int main(void) {
  unsigned int sizes[] = {10, 20, 30, 15, 25, 35};
  int n = sizeof(sizes) / sizeof(sizes[0]);
  bestfit(sizes, n);
  return 0;
}

void bestfit(unsigned int sizes[], int n) {
  // Iterate through the array of sizes
  for (int i = 0; i < n; i++) {
    unsigned int nbytes = sizes[i];
    printf("Allocating %d bytes of memory...\n", nbytes);

    // Call your malloc function
    void *ptr = malloc(nbytes);

    // Check if allocation was successful
    if (ptr == (void*)0) {
      printf("Memory allocation failed for %d bytes\n", nbytes);
    } else {
      printf("Memory allocated successfully at address: %p\n", ptr);
    }
  }
  return;
}