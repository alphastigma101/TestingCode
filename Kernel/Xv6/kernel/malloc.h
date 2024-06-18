#ifndef MALLOC_H
#define MALLOC_H
#include "kernel/types.h"
union header {
  // union type creates a user-defined type
  // It does not allocate any memory 
  // You need to create variables inside of it in order to allocate memory
  struct {
    union header *ptr; // Make a pointer to the header 
     unsigned int size; // The size used for the block 
  } s; // Declare an anonymous structure. Useful during a limited scope
  Align x; // 32bit int 
};
typedef union header Header;
struct LinkedList {
  Header *head;
  Header *next;
  Header *tail;
};

/* Function signatures */
void* malloc(uint64 nbytes);
Header* morecore(uint64 nu);
void free(void* ap);
void* allocation(uint64 size);



#endif // MALLOC_H