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

static Header p;
static Header *prevp = (void*)0; // This is the null
static Header bestp;

void* umalloc(unsigned int nbytes) {
  /* 
    - OS must maintain a list of free memory blocks available in the system. 
    - When a process requests memory, os searches this list for the smallest free block of memory that is large enough to accommodate the process. 
    - If a suitable block is found, the process is allocated memory from that block. If no suitable block is found, the operating system can either wait until a suitable block becomes available or request additional memory from the system.
  */
  
  unsigned int nunits; // Note: Need to change the types back to uint 
  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
  if(prevp == (void*)0) {
    prevp = &base;
    prevp->s.size = nunits;
    prevp->s.ptr = &p; // connect prevp to p through memory referencing
    p.s.ptr = &bestp; // connect p to best through memory referencing 
    base.s.size = nunits; // This initalizes all the linked lists sizes to zero
  }
  if (prevp->s.size != nunits) {
    prevp->s.size = nunits;
  }
  for(; (prevp->s.ptr) != (void*)0; prevp = prevp->s.ptr) {
    /*
      Layout:
        - [prevp|size] -> [p|size] -> [bestp|size]
      Notes:
        - Each node has a pointer that type that points to it's own node respectively 
    */
    if(p.s.size < prevp->s.size)  {
      printf("=======================================================\n");
      printf("Found a block that can accommodate %u units\n", nunits);
      printf("=======================================================\n");
      bestp.s.size = p.s.size;
      p.s.size = prevp->s.size;
      freep = prevp;
      return (void*)(&bestp + 1);  // Return address after the header
    }
    if (p.s.size < bestp.s.size) {
      bestp.s.size = p.s.size;
      freep = &bestp;
      return (void*)(&bestp + 1);  // Return address after the header
    }
    if (prevp->s.ptr->s.size == nunits && prevp->s.ptr->s.ptr->s.size == nunits) {
      printf("=======================================================\n");
      printf("Exact fit found, splitting the block\n");
      printf("Previous block size: %u\n", prevp->s.ptr->s.size);
      printf("Current block size: %u\n", p.s.size);
      printf("Next block pointer: %p\n", p.s.ptr);
      printf("=======================================================\n");
      prevp->s.ptr->s.ptr->s.size /= 2;
      return (void*)(prevp + 1); // return the linked list
    }
    bestp.s.size = p.s.size;
    p.s.size = prevp->s.size;
    freep = &bestp;
    return (void*)(&bestp + 1);
  }
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
