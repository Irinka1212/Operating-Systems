// Зад. 63 2017-SE-04 Напишете програма на C, която да работи подобно на командата cat, реализирайки само следната функционалност:
// • програмата извежда на STDOUT
// • ако няма подадени параметри, програмата чете от STDIN
// • ако има подадени параметри – файлове, програмата последователно ги извежда
// • ако някой от параметрите започва с тире (-), програмата да го третира като специално име
// за STDIN Примерно извикване: $ ./main f - g – извежда съдържанието на файла f, после STDIN, след това съдържанието на файла g.

#include <err.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char* argv[])
{
    int i = 1;
    bool hasParameters = (argc > 1);

    while (argv[i] != NULL || (!hasParameters && i == 1))
    {
        int fd;

        if ((argv[i] == NULL && !hasParameters) || (strcmp(argv[i], "-") == 0))
        {
            fd = 0;  
        }
        else
        {
            fd = open(argv[i], O_RDONLY);
            if (fd == -1)
            {
                err(3, "Cannot open file: %s", argv[i]);
                ++i;
                continue;
            }
        }

        ssize_t read_bytes;
        char c[4096];

        //setbuff(stdout, NULL);

        while ((read_bytes = read(fd, c, sizeof(c))) > 0)
        {
            ssize_t write_bytes = write(1, c, read_bytes);
            if (write_bytes == -1 || write_bytes != read_bytes)
            {
                err(1, "Error writing");
                break;
            }
        }

        if (read_bytes == -1)
        {
            err(2, "Error reading");
        }

        if (fd != 0)
        {
            close(fd);
        }

        ++i;
    }

    exit(0);
}
