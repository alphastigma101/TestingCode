// Long-term locks for processes
typedef struct sleeplock {
  uint locked;       // Is the lock held?
  spin_t lk; // spinlock protecting this sleep lock
  
  // For debugging:
  char *name;        // Name of lock.
  int pid;           // Process holding lock
} sleep_t;

