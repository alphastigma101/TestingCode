#include "kernel/types.h"
typedef struct hash_table_entry {
    hte_t *entries[ENTRIES]; // The amount of tables you can create
    uint64 key[ENTRIES]; // key entries 
    uint64 val[ENTRIES]; // val entries
    int *size; 
} __attribute__((aligned(ENTRIES))) aligned_hash_table_entries;

typedef struct hash_table {
    hte_t *table; // decalre hash table entry 
    uint64 (*getIndex)(hte_t *table, uint64 key); 
    void (*insert)(hte_t* table, uint64 key, uint64 value);
    uint64 (*getVal)(hte_t* table, uint64 key);
    uint64 (*getKey)(hte_t* table, uint64 key);
}  __attribute__((aligned(HASH_TABLE_MAX_SIZE))) aligned_hash_table;

/*  Struct hash table signatues... Should function like methods */
extern uint64 getIndex(hte_t* table, uint64 key); 
extern void insert(hte_t* table, uint64 key, uint64 value);
extern uint64 getVal(hte_t* table, uint64 key);
extern uint64 getKey(hte_t* table, uint64 key);
extern void table_alloc();


extern hte_t hash_entry_table[2];
extern hash_t ht;
