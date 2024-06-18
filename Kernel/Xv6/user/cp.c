#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"



void cp(const char *des_1, const char* des_2) {
    int src_fd, dest_fd;
    char buffer[512]; // Create a Buffer Stack of characters
    unsigned int bytes_read, bytes_written;

    src_fd = open(des_1, O_RDONLY);
    if (src_fd < 0) {
        fprintf(2, "cp: cannot open %s\n", des_1[1]);
        exit(1);
    }
    // Proper Syntax for open is: open(int fildes) which stands for file destination
    // fildes also is used by close, write, and read
    dest_fd = open(des_2, O_WRONLY | O_CREATE | O_TRUNC);
    if (dest_fd < 0) {
        fprintf(2, "cp: cannot create %s\n", des_2); // 2 = stderr
        exit(1);
    }

    while ((bytes_read = read(src_fd, buffer, sizeof(buffer))) > 0) {
        bytes_written = write(dest_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            fprintf(2, "cp: error writing to %s\n", des_2); // 2 = stderr
            exit(1);
        }
    }

    if (bytes_read < 0) {
        fprintf(2, "cp: error reading from %s\n", des_1);
        exit(1);
    }
    // Close the bytes/files
    close(src_fd); 
    close(dest_fd);
}
// Driver Code
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(2, "Usage: cp source_file destination_file\n");
        exit(1);
    }
    cp(argv[1], argv[2]);
    exit(0);
}

// Info: https://pubs.opengroup.org/onlinepubs/7908799/xsh/open.html
// To look for examples for something, right click it, and click on references 
// What are streams: a stream refers to a sequence of characters that are transferred between the program and input/output (I/O) devices. 
// https://www.udacity.com/blog/2023/04/c-streams.html