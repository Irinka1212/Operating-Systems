// Зад. 64 2018-SE-01 Напишете програма на C, която да работи подобно на командата tr, реализирайки само следната функционалност:
// • програмата чете от стандартния вход и пише на стандартния изход
// • общ вид на изпълнение: ./main [OPTION] SET1 [SET2]
// • OPTION би могъл да бъде или -d, или -s, или да липсва
// • SET1 и SET2 са низове, в които знаците нямат специално значение, т.е. всеки знак “означава” съответния знак. SET2, ако е необходим, трябва да е същата дължина като SET1
// • ако е подаден като първи параметър -d, програмата трие от входа всяко срещане на знак µ от SET1; SET2 не е необходим
// • ако е подаден като първи параметър -s, програмата заменя от входа всяка поредица от повторения знак µ от SET1 с еднократен знак µ; SET2 не е необходим
// • в останалите случаи програмата заменя от входа всеки знак µ от SET1 със съответстващият му позиционно знак ν от SET2.
// Примерно извикване:
// $ echo asdf | ./main -d ’d123’ | ./main ’sm’ ’fa’ | ./main -s ’f’
// af

#include <err.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        char buf[4096];

        ssize_t read_bytes;

        while( (read_bytes = read(0, &buf, sizeof(buf))) > 0)
        {
            if(write(1, &buf, read_bytes) != read_bytes)
            {
                err(1, "Error writing");
            }
        }

        if(read_bytes == -1)
        {
            err(2, "Error reading");
        }

        exit(0);
    }

    if (strcmp(argv[1], "-d") == 0)
    {
        if (argc != 3)
        {
            errx(3, "Arguments count");
        }

        char* set1 = argv[2];
        char buf[4096];

        ssize_t read_bytes;

        while ((read_bytes = read(0, buf, sizeof(buf))) > 0)
        {
            for (ssize_t i = 0; i < read_bytes; ++i)
            {
                if (strchr(set1, buf[i]) == NULL)
                {
                    if (write(1, &buf[i], 1) != 1)
                    {
                        err(1, "Error writing");
                    }
                }
            }
        }

        if (read_bytes == -1)
        {
            err(2, "Error reading");
        }
    }
    else if (strcmp(argv[1], "-s") == 0)
    {
        if (argc != 3)
        {
             errx(3, "Arguments count");
        }

        char* set1 = argv[2];
        char buf[4096];
        int prev_char = -1;

        ssize_t read_bytes;

        while ((read_bytes = read(0, buf, sizeof(buf))) > 0)
        {
            for (ssize_t i = 0; i < read_bytes; ++i)
            {
                if (strchr(set1, buf[i]) != NULL)
                {
                    if (buf[i] != prev_char)
                    {
                        if (write(1, &buf[i], 1) != 1)
                        {
                            err(1, "Error writing");
                        }
                    }
                    prev_char = buf[i];
                }
                else
                {
                    if (write(1, &buf[i], 1) != 1)
                    {
                        err(1, "Error writing");
                    }
                    prev_char = -1;
                }
            }
        }

        if (read_bytes == -1)
        {
            err(2, "Error reading");
        }
    }
    else
    {
        if (argc != 3)
        {
            errx(3, "Arguments count");
        }

        char* set1 = argv[1];
        char* set2 = argv[2];
        char buf[4096];

        ssize_t read_bytes;

        while ((read_bytes = read(0, buf, sizeof(buf))) > 0)
        {
            for (ssize_t i = 0; i < read_bytes; ++i)
            {
                char* pos = strchr(set1, buf[i]);
                if (pos != NULL)
                {
                    ssize_t index = pos - set1;
                    if (write(1, &set2[index], 1) != 1)
                    {
                        err(1, "Error writing");
                    }
                }
                else
                {
                    if (write(1, &buf[i], 1) != 1)
                    {
                        err(1, "Error writing");
                    }
                }
            }
        }

        if (read_bytes == -1)
        {
            err(2, "Error reading");
        }
    }

    exit(0);
}