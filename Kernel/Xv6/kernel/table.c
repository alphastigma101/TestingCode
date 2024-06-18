#include <kernel/table.h>
#include <kernel/malloc.h>
void printf(char*, ...);
static int curr_map = 0; // Should move this into the types.h without the static variable type. Place it with extern instead 
static int size = 0; // Should move this into the types.h without the static variable type. Place it with extern instead 
aligned_hash_table_entries hash_entry_table[2];
hash_t ht;

void table_alloc() {
  printf("Executing!\n");
  if (hash_entry_table[1].entries == (void*)0) {
    *hash_entry_table[1].entries = (hte_t*)malloc(sizeof(struct hash_table_entry));
    if (hash_entry_table[1].entries == (void*)0) {
      //panic("Failed to initialize table");
      printf("Failed to initialize table\n");
      return;

    }
  }
  if (ht.table == (void*)0) {
    ht.table = &hash_entry_table[1];
    if (ht.table == (void*)0) {
      //panic("Failed to initialize hash table");
      printf("Failed to initialize hash table");
      return;
    }
  }

  /* Initialize all hashed table methods */
  ht.getIndex = &getIndex;
  ht.insert = &insert;
  ht.getVal = &getVal;
  ht.getKey = &getKey;
}

uint64 getIndex(hte_t* table, uint64 key) { 
  // Search for the key to see if it exists
  for (int i = 0; i < HASH_TABLE_MAX_SIZE; i++) {
    if (table->entries[curr_map]->key[i] == key) { 
      if (PRINT == 1) {
        printf("Found key at index: %d\n", i);
        printf("=================\n");
      }
      return i; 
    }
  }
  return -1; // Key not found 
}

// Function to insert a key-value pair into the map 
void insert(hte_t* table, uint64 key, uint64 value)  {
  if (table->entries == (void*)0) {
    printf("table.c insert function: table->entries is void");
  }
  int index = getIndex(table, key);
  if (index == -1) { 
    // Key not found 
    if (PRINT == 0) {
      printf("Warning: Key was not found in table...\n"); 
      printf("Attempting to Add entries into the hash table..\n");
      printf("===================\n");
      printf("size of table is: %d\n", size);
    }
    if (size < HASH_TABLE_MAX_SIZE) {
      table->entries[size]->key[size] = key;
      table->entries[size]->val[size] = value;
      size++;
      table->size = &size;
      if (PRINT == 0) { 
        printf("Succesfully inserted key and value into table\n"); 
        printf("size of entries is now: %d\n", size);
      }
    }
    else {
      printf("Table is full... Not inserting key\n");
      return;
    }
  } 
  else { 
    printf("Key was found!\n");
    return;
  } 
}  

uint64 getVal(hte_t* table, uint64 key) { 
  int index = getIndex(table, key); 
  if (index == -1) { // Key not found 
    if (PRINT == 1) {
      printf("key was not found\n");
      printf("key: %d\n", key);
      printf("==============================\n");
    }
    return -1; 
  } 
  else { // Key found 
    printf("Key was found\n");
    return table->entries[curr_map]->val[index];
  } 
}
 
uint64 getKey(hte_t* table, uint64 key) { 
  int index = getIndex(table, key); 
  if (index == -1) { // Key not found 
    if (PRINT == 1) {
      printf("key was not found\n");
      printf("key: %d\n", key);
      printf("==============================\n");
    }
    return -1; 
  } 
  else { // Key found 
    printf("Key was found\n");
    return table->entries[curr_map]->key[index];
  } 
} 