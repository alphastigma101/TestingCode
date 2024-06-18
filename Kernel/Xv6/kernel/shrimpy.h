#include "kernel/table.h"

/* Function Signatures for shrimpy struct */
void map_frames();
void map_frames_helper();

// translate function helps with mapping when updating the page table
extern uint64 translate(pgt_t table, uint64  vaddr, int level); // Translate the virtual address to physical address
typedef struct shrimpy_t {
  proc_t *P; // assign this object with the parent process
  proc_t *NP;
  void* (*create_page_table)(); // create the mapping table
  void (*printhash)(pgt_t* table);
  void (*map_frames_helper)(); // Create the mapping for P,FRY to NP,SHRIMP
  void (*map_frames)();
  uint64 vas[7]; 
  uint64 *pas[7];
  uint64 (*translate)(pgt_t table, uint64 vaddr, int level);
  Align x; // 32bit int 
  //int TT[]; // OK, equivalent to int i[1] = {0}; unless redeclared later in this file
 
}__attribute__((aligned(4096))) shrimpy_t;

extern shrimpy_t *Shrimpy;