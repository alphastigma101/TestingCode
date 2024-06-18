xv6 is a re-implementation of Dennis Ritchie's and Ken Thompson's Unix
Version 6 (v6).  xv6 loosely follows the structure and style of v6,
but is implemented for a modern RISC-V multiprocessor using ANSI C.

ACKNOWLEDGMENTS

xv6 is inspired by John Lions's Commentary on UNIX 6th Edition (Peer
to Peer Communications; ISBN: 1-57398-013-7; 1st edition (June 14,
2000)).  See also https://pdos.csail.mit.edu/6.1810/, which provides
pointers to on-line resources for v6.

The following people have made contributions: Russ Cox (context switching,
locking), Cliff Frey (MP), Xiao Yu (MP), Nickolai Zeldovich, and Austin
Clements.

We are also grateful for the bug reports and patches contributed by
Takahiro Aoyagi, Silas Boyd-Wickizer, Anton Burtsev, carlclone, Ian
Chen, Dan Cross, Cody Cutler, Mike CAT, Tej Chajed, Asami Doi,
eyalz800, Nelson Elhage, Saar Ettinger, Alice Ferrazzi, Nathaniel
Filardo, flespark, Peter Froehlich, Yakir Goaron, Shivam Handa, Matt
Harvey, Bryan Henry, jaichenhengjie, Jim Huang, Matúš Jókay, John
Jolly, Alexander Kapshuk, Anders Kaseorg, kehao95, Wolfgang Keller,
Jungwoo Kim, Jonathan Kimmitt, Eddie Kohler, Vadim Kolontsov, Austin
Liew, l0stman, Pavan Maddamsetti, Imbar Marinescu, Yandong Mao, Matan
Shabtay, Hitoshi Mitake, Carmi Merimovich, Mark Morrissey, mtasm, Joel
Nider, Hayato Ohhashi, OptimisticSide, Harry Porter, Greg Price, Jude
Rich, segfault, Ayan Shafqat, Eldar Sehayek, Yongming Shen, Fumiya
Shigemitsu, Cam Tenny, tyfkda, Warren Toomey, Stephen Tu, Rafael Ubal,
Amane Uehara, Pablo Ventura, Xi Wang, WaheedHafez, Keiichi Watanabe,
Nicolas Wolovick, wxdao, Grant Wu, Jindong Zhang, Icenowy Zheng,
ZhUyU1997, and Zou Chang Wei.


The code in the files that constitute xv6 is
Copyright 2006-2022 Frans Kaashoek, Robert Morris, and Russ Cox.

### ERROR REPORTS

Please send errors and suggestions to Frans Kaashoek and Robert Morris
(kaashoek,rtm@mit.edu).  The main purpose of xv6 is as a teaching
operating system for MIT's 6.1810, so we are more interested in
simplifications and clarifications than new features.

### BUILDING AND RUNNING XV6

You will need a RISC-V "newlib" tool chain from
https://github.com/riscv/riscv-gnu-toolchain, and qemu compiled for
riscv64-softmmu.  Once they are installed, and in your shell
search path, you can run "make qemu".
* ------------------------------------------------------------------------------------------------------------------------------------------------------

# Ps utility 








* -------------------------------------------------------------------------------------------------------------------------------------------------------



# FatCat Scheduler




* -------------------------------------------------------------------------------------------------------------------------------------------------------


# Best fit


* ------------------------------------------------------------------------------------------------------------------------------------------------------

# Shrimpy 
* **Isolation**:
    - with pagetable, OS provides each process with its own private address space and memory. pagetable determine how to access physical memory and what parts can be accessed. guarding kernel and user stack with an unmapped page. 
* **(Guard page) Attention**: xv6 provides a single kernel pagetable for all processes. But each process has a user pagetable.

* **Multiplex**:
- mapping same physical memory in several address spaces. (like trampoline page)

* **Indirection**:
- contiguous virtual memory may mapping to uncontiguous physical memory


* **Page**:
- A page table gives OS control over virtual-to-physical address translations at the granularity of aligned chunks of 4096 bytes, such a chunk is called a page.
* **PTE to physical address**:
    - page is aligned chunk, so it must has contiguous memory, and it makes offset available.
    - entry of the page is given by PPN (middle 44-bit of PTE).
    - 12-bit offset from virtual address indicates offset from entry of this page.
    - 44-bit PPN + 12-bit offset is the 56-bit physical address we need.
* **Sources:**
    - *https://en.cppreference.com/w/c/memory/malloc*
    - *https://github.com/guisongchen/notes-of-6.S081/blob/main/pagetable.md*
    - *https://github.com/guisongchen/notes-of-6.S081/blob/main/pagetable.md#isolation*
    - *https://stackoverflow.com/questions/71583679/why-return-does-not-exit-a-process-in-xv6*
    - *https://en.cppreference.com/w/c/language/object#Alignment*
    - *https://en.cppreference.com/w/cpp/string/byte/memmove*
    - *https://en.cppreference.com/w/c/language/_Alignof*
    - *https://en.cppreference.com/w/cpp/string/byte/memset*
    - *https://en.cppreference.com/w/c/language/object*
    - *https://en.cppreference.com/w/c/language/typedef*
    - *https://eng.libretexts.org/Courses/Delta_College/C___Programming_I_(McClanahan)/12%3A_Pointers/12.07%3A_Function_Pointer_in_C*
    - *https://www.gnu.org/software/c-intro-and-ref/manual/html_node/Function-Pointers.html*
    - *https://www.cprogramming.com/tutorial/function-pointers.html*
* -------------------------------------------------------------------------------------------------------------------------------------------------------

# File based encryption
* **Brief Intro:***
    - In this lab your task is to implement a simple version of file-based encryption. The cryptography is laughable, but the OS picture resembles reality. The idea is to mark certain directories as **vaults.** Any **file written to a vault is "encrypted" by adding a constant to the bytes before they are written to disk.** **This constant serves as a key.**
* **Objective:**
    - The **stat command.**
    - A command for creating a vault from an empty directory. This should prompt for a key.
    - A command for opening a vault that has been created. This should prompt for a key.
    - A command for closing a vault. This should remove the key from memory.
* -------------------------------------------------------------------------------------------------------------------------------------------------------
# Inodes:
*  **Based on the directory tree**, the **low-level name of a file** is often referred to as its inode number (i-number). 
// An inode describes a single unnamed file.
// The inode disk structure holds metadata: the file's type,
// its size, the number of links referring to it, and the
// list of blocks holding the file's content.
//
// The inodes are laid out sequentially on disk at block
// sb.inodestart. Each inode has a number, indicating its
// position on the disk.
//
// The kernel keeps a table of in-use inodes in memory
// to provide a place for synchronizing access
// to inodes used by multiple processes. The in-memory
// inodes include book-keeping information that is
// not stored on disk: ip->ref and ip->valid.
//
// An inode and its in-memory representation go through a
// sequence of states before they can be used by the
// rest of the file system code.
//
// * Allocation: an inode is allocated if its type (on disk)
//   is non-zero. ialloc() allocates, and iput() frees if
//   the reference and link counts have fallen to zero.
//
// * Referencing in table: an entry in the inode table
//   is free if ip->ref is zero. Otherwise ip->ref tracks
//   the number of in-memory pointers to the entry (open
//   files and current directories). iget() finds or
//   creates a table entry and increments its ref; iput()
//   decrements ref.
//
// * Valid: the information (type, size, &c) in an inode
//   table entry is only correct when ip->valid is 1.
//   ilock() reads the inode from
//   the disk and sets ip->valid, while iput() clears
//   ip->valid if ip->ref has fallen to zero.
//
// * Locked: file system code may only examine and modify
//   the information in an inode and its content if it
//   has first locked the inode.
//
// Thus a typical sequence is:
//   ip = iget(dev, inum)
//   ilock(ip)
//   ... examine and modify ip->xxx ...
//   iunlock(ip)
//   iput(ip)
//
// ilock() is separate from iget() so that system calls can
// get a long-term reference to an inode (as for an open file)
// and only lock it for short periods (e.g., in read()).
// The separation also helps avoid deadlock and races during
// pathname lookup. iget() increments ip->ref so that the inode
// stays in the table and pointers to it remain valid.
//
// Many internal file system functions expect the caller to
// have locked the inodes involved; this lets callers create
// multi-step atomic operations.
//
// The itable.lock spin-lock protects the allocation of itable
// entries. Since ip->ref indicates whether an entry is free,
// and ip->dev and ip->inum indicate which i-node an entry
// holds, one must hold itable.lock while using any of those fields.
//
// An ip->lock sleep-lock protects all ip-> fields other than ref,
// dev, and inum.  One must hold ip->lock in order to
// read or write that inode's ip->valid, ip->size, ip->type, &c.

* -----------------------------------------------------------------------------------------------------------------------------

System Limits: Check the maximum allocation size supported by your system. This limit can vary based on the operating system and architecture. For example, on a 32-bit system, the maximum allocation size might be limited to 2GB or 4GB, while on a 64-bit system, it can be much larger.
Available Memory: Consider the amount of available memory on the system. Attempting to allocate more memory than is physically available can lead to performance issues, including swapping to disk and increased fragmentation.
Fragmentation: Large allocations can lead to memory fragmentation, especially in systems with limited virtual memory or in long-running applications. Fragmentation can degrade performance and increase the likelihood of memory allocation failures.
Cache Performance: Larger allocations may not fit efficiently into CPU caches, leading to increased cache misses and slower performance. Consider the cache hierarchy of your system when determining allocation sizes.
Application Requirements: Consider the specific requirements of your application. If your application needs to allocate large contiguous blocks of memory, you may need to adjust the allocation size accordingly.
Dynamic Allocation: Instead of allocating a single large block of memory, consider breaking it into smaller chunks or using dynamic allocation techniques like memory pools or slab allocators.
Benchmarking: Benchmark different allocation sizes to determine the optimal size for your application. Measure the performance impact of different allocation sizes under typical workload scenarios.


