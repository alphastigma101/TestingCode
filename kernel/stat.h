#include "kernel/types.h"
#define T_DIR     1   // Directory
#define T_FILE    2   // File
#define T_DEVICE  3   // Device

typedef struct stat {
  int dev;     // File system's disk device
  uint ino;    // Inode number
  short type;  // Type of file
  short nlink; // Number of links to file
  uint64 size; // Size of file in bytes
  int is_vault; // This will be set to one if a vault exists
  int vault_id; // This is the vault id 
  int vault_open;
} stat_t;

extern stat_t *st; // declare st as global
