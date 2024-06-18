#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(2, "Usage: touch name_of_file with any type of extension\n");
        exit(1);
    }
    touch(argv[1]);
    exit(0);
}

void touch(const char *des_1) {
    int dest;
    char buffer[512]; // Create a Buffer Stack of characters
    unsigned int bytes_read, bytes_written;
    dest = open(des_1, O_CREATE); // Create the folder or file
    while ((bytes_read = read(dest, buffer, sizeof(buffer))) > 0) {
        // Read all the bytes until it is zero 
        bytes_written = write(dest, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            fprintf(2, "touch: error creating %s\n", dest); // 2 = stderr
            exit(1);
        }
    }

}