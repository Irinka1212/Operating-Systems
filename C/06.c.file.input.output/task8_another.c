//in-place
#include <fcntl.h>
#include <err.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    int fd;
    char c;

    if ((fd = open("passwd", O_RDWR)) == -1) {
        err(2, "Cannot open file");
    }

    while (read(fd, &c, sizeof(c)) == sizeof(c)) {
        if (c == ':') {
            c = '?';
            if (lseek(fd, -1, SEEK_CUR) == -1) {
                err(3, "Error while seeking in file");
            }
            if (write(fd, &c, sizeof(c)) == -1) {
                err(4, "Error while writing to file");
            }
        }
    }

    close(fd);
    exit(0);
}