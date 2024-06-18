#define FATCAT_SCHEDULER 1
#define SHRIMPY 1
#define ALLOC 10096
#define HASH_TABLE_MAX_SIZE 4096
#define ENTRIES 512
#define PRINT 1

typedef struct proc proc_t;
typedef unsigned int  uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int  uint32;
typedef unsigned long uint64;
typedef uint64 size_t;

// Forward declaration of structures
// These can also be used to for function return types
struct stat;
struct mkvault;
struct sleeplock;
struct spinlock;
struct inode;
struct hash_table;
struct hash_table_entry;

// Typedefs for forward-declared structures
typedef long Align;
typedef uint64 *pgt_t;
typedef struct mkvault mkvault_t; // This is going to be used used for parameters 
typedef struct hash_table hash_t;
typedef struct hash_table_entry hte_t;
typedef uint64 pde_t;
typedef struct stat stat_t;
typedef struct sleeplock sleep_t; 
typedef struct spinlock spin_t;
typedef struct inode inode_t;