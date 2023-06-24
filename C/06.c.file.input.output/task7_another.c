//without output file

#include <fcntl.h>
#include <err.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    int i = 1;
    int output = open(argv[argc - 1], O_RDWR);
    if (output == -1) {
        err(2, "Cannot open file");
    }

    while (i < argc - 1)
    {
        int file = open(argv[i], O_RDONLY);
        if (file == -1) {
            err(2, "Cannot open file");
            i++;
            continue;
        }
	
        char c[4096];
        ssize_t read_size;

        while ((read_size = read(file, &c, sizeof(c))) > 0) {
            ssize_t write_size = write(output, &c, read_size);
            if (write_size == -1) {
                err(3, "Error while writing to standard output");
		i++;
	    	continue;
            }
        }

        if (read_size == -1) {
            err(4, "Error while reading from file");
	    i++;
	    continue;
        }

        close(file);
        i++;
    }
	
    close(output);
    exit(0);
}