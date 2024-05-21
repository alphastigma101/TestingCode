#include <stdio.h>

// Memory allocator by Kernighan and Ritchie,
// The C programming Language, 2nd ed.  Section 8.7.
#define ALLOC 4096
typedef long Align;

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
  int set;
};

static Header base;
static Header *freep;

void free(void *ap) {
  Header *bp, *p;
  bp = (Header*)ap - 1;
  //printf("bp: %p\n", bp);
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
      break;
  if(bp + bp->s.size == p->s.ptr){
    bp->s.size += p->s.ptr->s.size;
    bp->s.ptr = p->s.ptr->s.ptr;
  } else
    bp->s.ptr = p->s.ptr;
  if(p + p->s.size == bp){
    p->s.size += bp->s.size;
    p->s.ptr = bp->s.ptr;
  } else
    p->s.ptr = bp;
  freep = p;
}

static Header* morecore( unsigned int nu) {
  char *p;
  Header *hp;

  if (nu < 4096)
    nu = 4096;

  //printf("nu: %u\n", nu);

  p = sbrk(nu * sizeof(Header));
  //printf("p: %p\n", (void*)p);

  if (p == (char*)-1)
      return (void*)0;

  hp = (Header*)p;
  hp->s.size = nu;

  printf("hp->s.size: %u\n", hp->s.size);

  free((void*)(hp + 1));

  return freep;
}
// Static memory
static char memory[ALLOC];
static size_t memory_index = 0;
static struct LinkedList *node;

// Allocate memory from the static memory
void* allocation(size_t size) {
  if (memory_index + size > ALLOC) {
        return (void*)0; // Out of memory
    }
    void* ptr = &memory[memory_index];
    memory_index += size;
    return ptr;
}
void* umalloc( unsigned int nbytes) {
  /* 
    - OS must maintain a list of free memory blocks available in the system. 
    - When a process requests memory, os searches this list for the smallest free block of memory that is large enough to accommodate the process. 
    - If a suitable block is found, the process is allocated memory from that block. 
    - If no suitable block is found, the operating system can either wait until a suitable block becomes available 
    - or request additional memory from the system.
  */
  if (node == (void*)0) {
    node = (struct LinkedList *)allocation(sizeof(struct LinkedList));
    node->head = (Header*)allocation(sizeof(Header));
    node->head->s.size = 512;
    node->next = (Header*)allocation(sizeof(Header));
    node->next->s.size = 1028;
    node->tail = (Header*)allocation(sizeof(Header));
    node->tail->s.size = 2048;
    node->head->s.ptr = node->next;
    node->next->s.ptr = node->tail;
    node->tail->s.ptr = (void*)0;
  }
  unsigned int nunits;
  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
  for(; ( node->head->s.ptr) != (void*)0; node->head = node->head->s.ptr ) {
    /*
      Layout:
        - [head|size] -> [next|size] -> [tail|size]
      Notes:
        - Each node has a pointer that type that points to it's own node respectively 
    */
    if( node->head->s.size > node->next->s.size && node->head->s.size >= nunits ) {
      printf("=======================================================\n");
      printf("Found a block that can accommodate %u units\n", nunits);
      printf("=======================================================\n");
      node->head->s.size -= nunits;
      freep = node->head; // head is the current best fit 
    }
    if(node->next->s.size > node->tail->s.size && node->next->s.size >= nunits) {
        printf("=======================================================\n");
        printf("Found the best fit for the accommodated block %u units\n", nunits);
        printf("=======================================================\n");
        printf("=======================================================\n");
        printf("Head block size: %d\n", node->head->s.size);
        printf("Next block size: %d\n", node->next->s.size);
        printf("Tail block size: %d\n", node->tail->s.size);
        printf("=======================================================\n");
        printf("Head block pointer: %p\n", node->head->s.ptr);
        printf("Next block pointer: %p\n", node->next->s.ptr); 
        printf("Tail block pointer: %p\n", node->tail->s.ptr);
        node->head->s.size -= nunits;
        freep = node->next;
        return (void*)(node->next + 1);
    }
    else {
      printf("=======================================================\n");
      printf("Splitting blocks\n");
      printf("=======================================================\n");
      node->tail->s.size -= nunits;
      freep = node->tail;
      /* Debugging: */
      printf("=======================================================\n");
      printf("Head block size: %d\n", node->head->s.size);
      printf("Next block size: %d\n", node->next->s.size);
      printf("Tail block size: %d\n", node->tail->s.size);
      printf("=======================================================\n");
      printf("Head block pointer: %p\n", node->head->s.ptr);
      printf("Next block pointer: %p\n", node->next->s.ptr); // Note: If you see next pointing to null that is good because that means the linked list has not grown yet
      printf("Tail block pointer: %p\n", node->tail->s.ptr); // Note: If you see tail pointing to null that is good because that means the linked list has not grown yet
      return (void*)(node->tail + 1);
    }
    if ( node->head->s.size == nunits && node->next->s.size == nunits && node->tail->s.size == nunits ) {
      printf("=======================================================\n");
      printf("Exact fit found, splitting the block\n");
      printf("=======================================================\n");
      node->head->s.size -= nunits;
      node->next->s.size -= nunits;
      node->tail->s.size -= nunits;
      freep = node->head;
      return (void*)(node->head + 1);
    }
    else {
      // Start adding sub nodes to the parent node (next)
      if (freep == (void*)0) {
        /* Start adding sub nodes to the parent node (next) if:
           1. freep was not set with a node from the linked list. This means that best fit was not found
           2. You want to make things harder
           3. Otherwise just keep adding nodes at the tail end
        */                                       
        /* New Layout:                           ^
                                                 |
                                       _______________________
                                      |       size, ptr       |
                                      |_______________________|
                                                  ^ 
                                                  |               
           _______________________       _______________________        _______________________       __________________
          |       size, ptr       | --->|       size, ptr       |----> |    size,ptr           | --> | size,ptr         |
          |_______________________|     |_______________________|      |_______________________|     |__________________|
                                                                                                     
        */                                             
        printf("=======================================================\n");
        printf("No suitable block found, making one\n");
        printf("=======================================================\n");
        freep = node->tail;
        node->tail->s.ptr = morecore(nunits);
        return (void*)(node->tail + 1);
      }
    }
  }
  return (void*)0;
}
int main(void) {
  unsigned int sizes[] = {10, 20, 30, 15, 25, 35};

  // Iterate through the array of sizes
  for (int i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i++) {
    unsigned int nbytes = sizes[i];
    printf("Allocating %d bytes of memory...\n", nbytes);

    // Call your malloc function
    void *ptr = umalloc(nbytes);

    // Check if allocation was successful
    if (ptr == (void*)0) {
        printf("Memory allocation failed for %d bytes\n", nbytes);
    } else {
        printf("Memory allocated successfully at address: %p\n", ptr);
    }
  }
  return 0;
}
