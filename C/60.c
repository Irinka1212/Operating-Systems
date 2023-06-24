// Зад. 60 2017-SE-01 Напишете програма на C, която приема три параметъра – имена на двоични файлове. Примерно извикване: $ ./main f1.bin f2.bin patch.bin
// Файловете f1.bin и f2.bin се третират като двоични файлове, състоящи се от байтове (uint8_t). Файлът f1.bin e “оригиналният” файл, а f2.bin е негово копие, което е било модифицирано по
// някакъв начин (извън обхвата на тази задача). Файлът patch.bin е двоичен файл, състоящ се от наредени тройки от следните елементи (и техните типове):
// • отместване (uint16_t) – спрямо началото на f1.bin/f2.bin
// • оригинален байт (uint8_t) – на тази позиция в f1.bin
// • нов байт (uint8_t) – на тази позиция в f2.bin
// Вашата програма да създава файла patch.bin, на базата на съществуващите файлове f1.bin и f2.bin, като описва вътре само разликите между двата файла. 
// Ако дадено отместване съществува само в единия от файловете f1.bin/f2.bin, програмата да прекратява изпълнението си по подходящ начин.
// Примерен f1.bin:
// 00000000: f5c4 b159 cc80 e2ef c1c7 c99a 2fb0 0d8c ...Y......../...
// 00000010: 3c83 6fed 6b46 09d2 90df cf1e 9a3c 1f05 <.o.kF.......<..
// 00000020: 05f9 4c29 fd58 a5f1 cb7b c9d0 b234 2398 ..L).X...{...4#.
// 00000030: 35af 6be6 5a71 b23a 0e8d 08de def2 214c 5.k.Zq.:......!L
// Примерен f2.bin:
// 00000000: f5c4 5959 cc80 e2ef c1c7 c99a 2fb0 0d8c ..YY......../...
// 00000010: 3c83 6fed 6b46 09d2 90df cf1e 9a3c 1f05 <.o.kF.......<..
// 00000020: 05f9 4c29 fd58 a5f1 cb7b c9d0 b234 2398 ..L).X...{...4#.
// 00000030: afaf 6be6 5a71 b23a 0e8d 08de def2 214c ..k.Zq.:......!L
// Примерен patch.bin:
// 00000000: 0200 b159 3000 35af ...Y0.5.

#include <err.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct data
{
    uint16_t offset;
    uint8_t originalPosition;
    uint8_t newPosition;

}__attribute__((packed)) Data;

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        errx(1, "We need 3 arguments");
    }

    int fd1 = open(argv[1], O_RDONLY);
    if (fd1 == -1)
    {
        err(2, "Cannot open file");
    }

    int fd2 = open(argv[2], O_RDONLY);
    if (fd2 == -1)
    {
        err(2, "Cannot open file");
    }

    int fd3 = open(argv[3], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
    if (fd3 == -1)
    {
        err(2, "Cannot open file");
    }

    Data data;

    struct stat st1, st2;
    if (fstat(fd1, &st1) == -1 || fstat(fd2, &st2) == -1)
    {
        err(3, "Error with fstat");
    }

    if (st1.st_size != st2.st_size)
    {
        errx(4, "Files have different sizes");
    }

    if (st1.st_size % sizeof(Data) != 0)
    {
        errx(5, "Error with file format");
    }

    for(data.offset = 0; data.offset < st1.st_size; ++data.offset)
    {
        if(read(fd1, &data.originalPosition, sizeof(data.originalPosition)) <= 0)
        {
            err(6, "Error reading");
        }

        if(read(fd2, &data.newPosition, sizeof(data.newPosition)) <= 0)
        {
            err(6, "Error reading");
        }

        if(data.originalPosition != data.newPosition)
        {
            if(write(fd3, &data, sizeof(data)) != sizeof(data))
            {
                 err(7, "Error writing");
            }
        }
    }

    close(fd1);
    close(fd2);
    close(fd3);
    exit(0);
}