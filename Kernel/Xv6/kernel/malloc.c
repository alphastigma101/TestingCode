#include "kernel/malloc.h"
#include "kernel/types.h"
void printf(char*, ...);


/* Linked list struct */
static struct LinkedList *node;
static Header *freep;
/* Static memory signatures */
static char memory[ALLOC];
static unsigned int memory_index = 0;


void free(void *ap) {
  Header *bp, *p;
  bp = (Header*)ap - 1;
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

Header* morecore(uint64 nu) {
  char *p;
  Header *hp;

  if (nu < 4096)
    nu = 4096;
  p = allocation(nu * sizeof(Header));

  if (p == (char*)-1) {
    printf("Failed to allocate memory!\n");
    return (void*)0;
  }

  hp = (Header*)p;
  hp->s.size = nu;

  free((void*)(hp + 1));

  return freep;
}


// Allocate memory from the static memory
void* allocation(uint64 size) {
  if (memory_index + size > ALLOC) {
    printf("Ran out of memory!\n");
    return (void*)0; // Out of memory
  }
  void* ptr = &memory[memory_index];
  memory_index += size;
  return ptr;
}

void* malloc(uint64 nbytes) {
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
  uint64 nunits;
  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
  for(; ( node->head->s.ptr) != (void*)0; node->head = node->head->s.ptr ) {
    /*
      Layout:
        - [head|size] -> [next|size] -> [tail|size]
      Notes:
        - Each node has a pointer that type that points to it's own node respectively 
    */
    if( node->head->s.size > node->next->s.size && node->head->s.size >= nunits ) {
      node->head->s.size -= nunits;
      freep = node->head; // head is the current best fit 
    }
    if(node->next->s.size > node->head->s.size && node->next->s.size >= nunits) {
      node->next->s.size -= nunits;
      freep = node->next;
      return (void*)(node->next + 1);
    } 
    if ( node->head->s.size == nunits && node->next->s.size == nunits ) {
      node->head->s.size -= nunits;
      node->next->s.size -= nunits;
      freep = node->head;
      return (void*)(node->head + 1);
    }
  } // outside of the for loop
  if (node->head->s.ptr == (void*)0) {
    if (freep == (void*)0) {                                            
      freep = node->tail;
      node->tail->s.ptr = morecore(nunits);
      return (void*)(node->tail + 1);
    }
    else {
      freep = node->tail;
      node->tail->s.ptr = morecore(nunits);
      return (void*)(node->tail + 1);
    }
  }
  return (void*)0;
}