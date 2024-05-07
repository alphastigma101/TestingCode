#include <stdio.h>

union header {
  // union type creates a user-defined type
  // It does not allocate any memory 
  // You need to create variables inside of it 
  struct {
    union header *ptr; // Make a pointer to the header 
    unsigned int size; // The size used for the block 
  } s; // Declare an anonymous structure. Useful during a limited scope
  unsigned int x; // 32bit int 
};

typedef union header Header;

static Header base;
static Header *freep;

void free(void *ap) {
  Header *bp, *p;

  bp = (Header*)ap - 1;
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
      break;
  if(bp + bp->s.size == p->s.ptr) {
    bp->s.size += p->s.ptr->s.size;
    bp->s.ptr = p->s.ptr->s.ptr;
  } 
  else
    bp->s.ptr = p->s.ptr;
  if(p + p->s.size == bp) {
    p->s.size += bp->s.size;
    p->s.ptr = bp->s.ptr;
  } 
  else
    p->s.ptr = bp;
  freep = p;
}

static Header* morecore(unsigned int nu) {
  char *p;
  Header *hp;

  if(nu < 4096)
    nu = 4096;
  p = sbrk(nu * sizeof(Header));
  if(p == (char*)-1)
    return 0;
  hp = (Header*)p;
  hp->s.size = nu;
  free((void*)(hp + 1));
  return freep;
}
static Header *p, *prevp, *bestp = (void*)0; // This is the null

void* malloc(unsigned int nbytes) {
  /* 
    - OS must maintain a list of free memory blocks available in the system. 
    - When a process requests memory, os searches this list for the smallest free block of memory that is large enough to accommodate the process. 
    - If a suitable block is found, the process is allocated memory from that block. If no suitable block is found, the operating system can either wait until a suitable block becomes available or request additional memory from the system.
  */
  
  unsigned int nunits; // Note: Need to change the types back to uint 
  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
  if((prevp = freep) == 0) {
    // Add another node that will store the best fit 
    base.s.ptr = freep = prevp = bestp = &base; // This is the linked list 
    base.s.size = 0; // This initalizes all the linked lists sizes to zero
  }
  if (prevp->s.size == 0) { prevp->s.size = nbytes; } // Always store the size
  for(p = prevp->s.ptr, bestp = p->s.ptr; ; prevp = p, p = p->s.ptr, bestp = bestp->s.ptr) {
    // [prevp] -> [p] -> [bestp]
    // prevp = p is a pointy that is incrementing through stored memory addresses 
    // p = prevp->s.ptr this is the previous node which is the base 
    // pevp = p // connect the nodes together
    // p = p->s.ptr
    if(prevp->s.size < p->s.size)  {
      printf("=======================================================\n");
      printf("Found a block that can accommodate %u units\n", nunits);
      printf("=======================================================\n");
      prevp = p; // Set the node that has the current best fit 
      if (p->s.size < bestp->s.size) {
        bestp = p;
        freep = bestp;
        return (void*)(bestp + 1);  // Return address after the header
      }
      bestp->s.size = prevp->s.size;
      freep = bestp;
      return (void*)(bestp + 1);  // Return address after the header
    }
    if (prevp->s.size < bestp->s.size) {
      bestp = prevp; // best fit 
      // Update freep
      freep = bestp; // Update the linked list so it is at the end
      return (void*)(bestp + 1); // return the linked list
    }
    if (prevp->s.size == nunits) {
      printf("=======================================================\n");
      printf("Exact fit found, adjusting pointers\n");
      printf("Previous block size: %u\n", prevp->s.size);
      printf("Current block size: %u\n", p->s.size);
      printf("Next block pointer: %p\n", p->s.ptr);
      printf("=======================================================\n");
      if (p->s.size == nunits) {
        prevp->s.ptr = p->s.ptr;
        p->s.ptr = bestp->s.ptr;
        freep = bestp;
        return (void*)(bestp + 1); // return the linked list
      }
    } 
    if (bestp == freep) {  
      // Reached the end of the list
      if (bestp == (void*)0) {
        // No suitable block found
        if ((bestp = morecore(nunits)) == (void*)0) {
          printf("Allocation failure\n");
          return (void*)(0);  // Allocation failure
        }
      }
    }
    else {
      // Print debug information
      printf("Previous block size: %d\n", prevp->s.size);
      printf("Current block size: %d\n", p->s.size);
      printf("Next block pointer: %p\n", p->s.ptr);
      printf("Number of blocks: %d\n", nunits);
      if (prevp->s.size >= p->s.size) {
        if (p->s.size >= bestp->s.size) {
          // Split the current block
          p->s.size -= nunits;
          prevp->s.size = nunits;
          bestp->s.size = p->s.size;
          freep = bestp;
          return (void*)(bestp + 1); // return the linked list
          
        }
      }
      else {
        prevp->s.size -= nunits;
        p->s.size += prevp->s.size; // append the previous block size
        if (p->s.size >= bestp->s.size) {
          p->s.size -= nunits; // split it because p->s.size is the smallest block size 
          prevp->s.size = nunits; // set the block back again which will hold the biggest block size
          bestp->s.size = p->s.size; // Update the bestp node
          freep = bestp; // Set the bestp node to be at the end of the linked list
          return (void*)(bestp + 1); // return the linked list
        }
        else {
          // All the nodes are greater than the split 
          // This means it is the smallest 
          bestp->s.size = prevp->s.size;
          freep = bestp;
          return (void*)(bestp + 1); // return the linked list
        }
      }
    }
  }
}
int main(void) {

    unsigned int sizes[] = {10, 20, 30, 15, 25, 35};

    // Iterate through the array of sizes
    for (int i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i++) {
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
    return 0;
}
