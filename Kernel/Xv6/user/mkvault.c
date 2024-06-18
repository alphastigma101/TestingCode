
#include "kernel/mkvault.h"
#include "user/user.h"
#include <stdarg.h>

mkvault_t *mkv;
stat_t *st;
static int MARKED = 0;


int snprintf(char * restrict dest, size_t n, const char * restrict format, ...) {
    va_list args;
    va_start(args, format);

    int chars_written = 0; // Number of characters written
    size_t dest_index = 0; // Current index in the destination buffer
    size_t arg_index = 0;  // Current index of argument being processed

    printf("snprintf called with dest: %p, n: %zu, format: %s\n", dest, n, format);

    // Iterate through the format string
    while (format[arg_index] != '\0' && dest_index < n - 1) {
        if (format[arg_index] == '%') {
            // If '%' is encountered, check for format specifiers
            arg_index++;
            printf("format[arg_index] is: %c\n", format[arg_index]);
            if (format[arg_index] == 'd') {
                // Handle %d specifier for integers
                int arg = va_arg(args, int);
                printf("Handling %%d with value: %d\n", arg);
                int digits = snprintf((void*)0, 0, "%d", arg);
                // Check if there's enough space in the destination buffer
                if (dest_index + digits < n - 1) {
                    // Write the integer to the destination buffer
                    dest_index += snprintf(dest + dest_index, n - dest_index, "%d", arg);
                    chars_written += digits;
                } else {
                    // Not enough space, break out of the loop
                    printf("Not enough space for integer\n");
                    break;
                }
            } else if (format[arg_index] == 's') {
                // Handle %s specifier for strings
                const char *str_arg = va_arg(args, const char *);
                printf("Handling %%s with value: %s\n", str_arg);
                // Write the string to the destination buffer
                while (*str_arg != '\0' && dest_index < n - 1) {
                    dest[dest_index++] = *str_arg++;
                    chars_written++;
                }
            }
            arg_index++;
        } else {
            // Copy regular characters to the destination buffer
            printf("Copying character: %c\n", format[arg_index]);
            dest[dest_index++] = format[arg_index++];
            chars_written++;
        }
    }

    // Null-terminate the destination buffer
    dest[dest_index] = '\0';

    va_end(args);

    printf("snprintf finished with dest: %s, chars_written: %d\n", dest, chars_written);

    return chars_written;
}

// Stand alone Function to mark a directory as a vault by creating a marker file
void mav(const char *path) {
    // Have to check and see if the directory is marked or not 
    // if it marked then do not mark it again 
    // Ask for key to decrypt it
    char marker_path[512];
    snprintf(marker_path, sizeof(marker_path), "%s/.vault", path);
    int fd = open(marker_path, O_CREATE | O_RDWR);
    if (fd < 0) {
        MARKED = -1;
    } 
    else {
        fprintf(1, "Marked %s as vault\n", path);
        close(fd);
        MARKED = 1;
    }
}
int main(int argc, char *argv[]) {
    char input[100]; // used to grab user input 
    int key;
    if (argc != 2) {
        fprintf(2, "Usage: mkvault name_of_directory \n");
        exit(1);
    }
    if (mkv == (void*)0) { mkv = (mkvault_t*)malloc(sizeof(mkvault_t)); }
    mkv->m.mav = &mav;
    if (mkvault((uint64)argv[1]) == -1) { exit(-1); }
    else {
        printf("Directory does not exist... creating one...\n");
        mkdir(argv[1]);
    }
    // Check to see if the directory is marked
    mkv->m.mav(argv[1]);
    if (MARKED == -1) {
        printf("Couldn't mark the directory\n");
        free(mkv);
        exit(-1);
    }

    // Print success message
    printf("Directory marked as vault successfully!\n");
    // Prompt the user to enter a key
    fprintf(1, "Enter a key: ");

    // Read user input
    int n = read(0, input, sizeof(input));
    if (n > 0) {
        // Remove newline character if present
        for (int i = 0; i < n; i++) {
            if (input[i] == '\n') {
                input[i] = '\0'; // add a null terminator 
                break;
            }
        }
        // Validate and convert the input to an integer
        key = atoi(input);
        if (key == 0 && input[0] != '0') {
            fprintf(1, "Invalid input. Please enter a valid integer.\n");
            exit(-1);
        }
        mkv->key = key;
    } 
    else {
        fprintf(1, "Error reading input.\n");
        exit(-1);
    }

    return 0;
}

// iupdate, iget is located in fs.c. There are really good notes in fs.c about inodes which you should grab
// ilock struct is located in defs.h
// create funciton is located in sysfile.c
// made my life eaiser by using grep -R 'what you're looking for here' ./path/to/file



// Notes from book:
// dinode is represents the directory tree 
// The i-node is the number is reffered to the low-level filename 
// unlink() is used to remove files
// Memory mapping is an alternative way to access persistent data in files.
// The mmap() system call creates a correspondence between byte offsets in
// a file and virtual addresses in the calling process; the former is called the
// backing file and the latter its in-memory image.

// To see the metadata for a certain file, we can use the
// stat() or fstat() system calls

// Code built in already into xv6 
/*
p = mmap(NULL, file_size, PROT_READ|PROT_WRITE,
2 MAP_SHARED, fd, 0);
3 assert(p != MAP_FAILED);
4 for (int i = 1; i < argc; i++)
5 if (strcmp(argv[i], "pop") == 0) // pop
6 if (p->n > 0) // stack not empty
7 printf("%d\n", p->stack[--p->n]);
8 } else { // push
9 if (sizeof(pstack_t) + (1 + p->n) * sizeof(int)
10 <= file_size) // stack not full
11 p->stack[p->n++] = atoi(argv[i]);
12 }
The program pstack.c (included on the OSTEP code github repo, with
a snippet shown above) stores a persistent stack in file ps.img, which
begins life as a bag of zeros, e.g., created on the command line via the
truncate or dd utility. The file contains a count of the size of the stack
and an array of integers holding stack contents. 
*/
/* 
Reading Directories
Now that we’ve created a directory, we might wish to read one too.
Indeed, that is exactly what the program ls does. Let’s write our own
little tool like ls and see how it is done.
Instead of just opening a directory as if it were a file, we instead use
a new set of calls. Below is an example program that prints the contents
of a directory. The program uses three calls, opendir(), readdir(),
and closedir(), to get the job done, and you can see how simple the
interface is; we just use a simple loop to read one directory entry at a time,
and print out the name and inode number of each file in the directory.
int main(int argc, char *argv[]) {
DIR *dp = opendir(".");
assert(dp != NULL);
struct dirent *d;
while ((d = readdir(dp)) != NULL) {
printf("%lu %s\n", (unsigned long) d->d_ino,
d->d_name);
}
closedir(dp);
return 0;
}
*/
/* 
The declaration below shows the information available within each
directory entry in the struct dirent data structure:
struct dirent {
char d_name[256]; // filename
ino_t d_ino; // inode number
off_t d_off; // offset to the next dirent
unsigned short d_reclen; // length of this record
unsigned char d_type; // type of file
};
Because directories are light on information (basically, just mapping
the name to the inode number, along with a few other details), a program
may want to call stat() on each file to get more information on each,
such as its length or other detailed information. Indeed, this is exactly
what ls does when you pass it the -l flag; try strace on ls with and
without that flag to see for yourself.
39.13 Deleting Directories
Finally, you can delete a directory with a call to rmdir() (which is
used by the program of the same name, rmdir). Unlike file deletion,
however, removing directories is more dangerous, as you could poten-
tially delete a large amount of data with a single command. Thus, rmdir()
has the requirement that the directory be empty (i.e., only has “.” and “..”
entries) before it is deleted. If you try to delete a non-empty directory, the
call to rmdir() simply will fail.
*/
// Code Snippets:
/*
    snippet of creating files: 

    int fd = open("foo", O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR|S_IWUSR);
*/
/*
Thus far, we’ve discussed how to read and write files, but all access
has been sequential; that is, we have either read a file from the beginning
to the end, or written a file out from beginning to end.
Sometimes, however, it is useful to be able to read or write to a spe-
cific offset within a file; for example, if you build an index over a text
document, and use it to look up a specific word, you may end up reading
from some random offsets within the document. To do so, we will use
the lseek() system call. Here is the function prototype:
off_t lseek(int fildes, off_t offset, int whence);
*/

// Important objects that are apart of struct proc: 
// struct file *ofile[NOFILE]; // Open files


// Tools you should look into: 

/*
The strace tool provides an awesome way to see what programs are up
to. By running it, you can trace which system calls a program makes, see
the arguments and return codes, and generally get a very good idea of
what is going on.
The tool also takes some arguments which can be quite useful. For ex-
ample, -f follows any fork’d children too; -t reports the time of day
at each call; -e trace=open,close,read,write only traces calls to
those system calls and ignores all others. There are many other flags; read
the man pages and find out how to harness this wonderful tool.
*/

// TODO:
// Copy the dinode here. If it does not exist 
// dinode might have these attributes:
// type <- the file extension
// You need to worry about the offset.. Basically the first two bits starting from the left would be the offset


/*
A file is an array of bytes which can be created, read, written, and
deleted. It has a low-level name (i.e., a number) that refers to it
uniquely. The low-level name is often called an i-number.
• A directory is a collection of tuples, each of which contains a
human-readable name and low-level name to which it maps. Each
entry refers either to another directory or to a file. Each directory
also has a low-level name (i-number) itself. A directory always has
two special entries: the . entry, which refers to itself, and the ..
entry, which refers to its parent.
• A directory tree or directory hierarchy organizes all files and direc-
tories into a large tree, starting at the root.
• To access a file, a process must use a system call (usually, open())
to request permission from the operating system. If permission is
granted, the OS returns a file descriptor, which can then be used
for read or write access, as permissions and intent allow.
• Each file descriptor is a private, per-process entity, which refers to
an entry in the open file table. The entry therein tracks which file
this access refers to, the current offset of the file (i.e., which part
of the file the next read or write will access), and other relevant
information.
• Calls to read() and write() naturally update the current offset;
otherwise, processes can use lseek() to change its value, enabling
random access to different parts of the file.
• To force updates to persistent media, a process must use fsync()
or related calls. However, doing so correctly while maintaining
high performance is challenging [P+14], so think carefully when
doing so.
• To have multiple human-readable names in the file system refer to
the same underlying file, use hard links or symbolic links. Each
is useful in different circumstances, so consider their strengths and
weaknesses before usage. And remember, deleting a file is just per-
forming that one last unlink() of it from the directory hierarchy.
• Most file systems have mechanisms to enable and disable sharing.
A rudimentary form of such controls are provided by permissions
bits; more sophisticated access control lists allow for more precise
control over exactly who can access and manipulate information.
*/

// inode which stands for index nodes is used for indexing into the disk partition's memory in the search for metadata 