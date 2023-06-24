// Зад. 86 2020-SE-03 Напишете програма на C, която приема един задължителен позиционен параметър - име на файл. Файлът се състои от не повече от 8 наредени тройки елементи:
// • име на файл – точно 8 байта, последният от които задължително е 0x00. Ако името е по-късо от 7 знака, излишните байтове са 0x00;
// • offset – uint32_t, който дава пореден номер на елемент (спрямо N0) във файла;
// • length – uint32_t, който дава брой елементи.
// За всяка наредена тройка програмата трябва да пусне child процес, който да XOR-не (обработи сизключващо-или) елементите (uint16_t) от съответния файл един със друг, 
// и да върне резултата на parent процеса, който от своя страна трябва да XOR-не всички получените резултати и да изведе полученото число в следния формат: result: 573B
// Забележка: С пълен брой точки се оценяват решения, в които child процесите работят паралелно.

#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct data
{
    char name[8];
    uint32_t offset;
    uint32_t length;

} data;

int main(int argc, char* argv[])
{

    if (argc != 2)
    {
        errx(1, "The argument needs to be one.");
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1)
    {
        err(2, "Cannot open file.");
    }

    data arr[8];

    struct stat st;
    if (fstat(fd, &st) == -1)
    {
        err(3, "Error stat");
    }

    if (st.st_size % sizeof(data) != 0 || st.st_size > sizeof(arr))
    {
        errx(4, "Incorrect format");
    }

    int count = st.st_size / sizeof(data);

    if (read(fd, arr, count * sizeof(data)) != count * sizeof(data))
    {
        err(5, "Error while reading");
    }

    close(fd);

    for (int i = 0; i < count; ++i)
    {
        if (arr[i].name[7] != '\x00')
        {
            errx(6, "Wrong file format");
        }
    }

    int pd[2];
    if (pipe(pd) == -1)
    {
        err(7, "Error with pipe");
    }

    int i = 0;
    pid_t pid;
    for (; i < count; ++i)
    {
        pid = fork();
        if (pid == -1)
        {
            err(8, "Error with fork");
        }

        if (pid == 0)
        {
            close(pd[0]);
            break;
        }
    }

    uint16_t r = 0x0000;
    if (pid == 0)
    {
        fd = open(arr[i].name, O_RDONLY);
        if (fd == -1)
        {
            err(2, "Cannot open file.");
        }

        if (lseek(fd, arr[i].offset * sizeof(uint16_t), SEEK_SET) == -1)
        {
            err(9, "Error with lseek");
        }

        uint16_t buf;
        for (int j = 0; j < arr[i].length; ++j)
        {
            if (read(fd, &buf, sizeof(buf)) == -1)
            {
                err(5, "Error while reading");
            }

            r ^= buf;
        }

        if (write(pd[1], &r, sizeof(r)) == -1)
        {
            err(10, "Error while writing");
        }

        close(pd[1]);
        exit(0);
    }

    r = 0x0000;
    for (int j = 0; j < count; ++j)
    {
        wait(NULL);
        uint16_t buf;
        close(pd[1]);

        if (read(pd[0], &buf, sizeof(buf)) == -1)
        {
            err(5, "Error while reading");
        }

        r ^= buf;
    }

    close(pd[0]);
    dprintf(1, "04%X\n", r);

    return 0;
}