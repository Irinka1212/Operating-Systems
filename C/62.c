// Зад. 62 2017-SE-03 Напишете програма на C, която приема три параметъра, имена на двоични файлове. Примерно извикване: $ ./main patch.bin f1.bin f2.bin
// Файловете f1.bin и f2.bin се третират като двоични файлове, състоящи се от байтове (uint8_t). Файлът patch.bin е двоичен файл, състоящ се от наредени тройки от следните елементи (и техните типове):
// • отместване uint16_t
// • оригинален байт uint8_t
// • нов байт uint8_t
// Програмата да създава файла f2.bin като копие на файла f1.bin, но с отразени промени на базата на файла patch.bin, при следния алгоритъм:
// • за всяка наредена тройка от patch.bin, ако на съответното отместване (в байтове) спрямо началото на файла е записан байта оригинален байт, в изходния файл се записва нов байт.
// Ако не е записан такъв оригинален байт или такова отместване не съществува, програмата да прекратява изпълнението си по подходящ начин;
// • всички останали байтове се копират директно.
// Забележка: Наредените тройки във файла patch.bin да се обработват последователно.
// Примерен f1.bin:
// 00000000: f5c4 b159 cc80 e2ef c1c7 c99a 2fb0 0d8c ...Y......../...
// 00000010: 3c83 6fed 6b46 09d2 90df cf1e 9a3c 1f05 <.o.kF.......<..
// 00000020: 05f9 4c29 fd58 a5f1 cb7b c9d0 b234 2398 ..L).X...{...4#.
// 00000030: 35af 6be6 5a71 b23a 0e8d 08de def2 214c 5.k.Zq.:......!L
// Примерен patch.bin:
// 00000000: 0200 b159 3000 35af ...Y0.5.
// Примерен f2.bin:
// 00000000: f5c4 5959 cc80 e2ef c1c7 c99a 2fb0 0d8c ..YY......../...
// 00000010: 3c83 6fed 6b46 09d2 90df cf1e 9a3c 1f05 <.o.kF.......<..
// 00000020: 05f9 4c29 fd58 a5f1 cb7b c9d0 b234 2398 ..L).X...{...4#.
// 00000030: afaf 6be6 5a71 b23a 0e8d 08de def2 214c ..k.Zq.:......!L

#include <err.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct data
{
    uint16_t offset;
    uint8_t originalByte;
    uint8_t newByte;

}__attribute__((packed)) Data;

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        errx(1, "Need 3 arguments");
    }

    int patch = open(argv[1], O_RDONLY);
    if (patch == -1)
    {
        err(2, "Cannot open");
    }

    int fd1 = open(argv[2], O_RDONLY);
    if (fd1 == -1)
    {
        err(2, "Cannot open");
    }

    int fd2 = open(argv[3], O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd2 == -1)
    {
        err(2, "Cannot open");
    }

    Data data;

    struct stat st1, st2;

    if (fstat(patch, &st1) == -1 || fstat(fd1, &st2) == -1)
    {
        err(3, "Error with fstat");
    }

    if (st2.st_size % sizeof(Data) != 0)
    {
        errx(4, "Error with file format");
    }

    ssize_t read_bytes;
    uint8_t buf;

    while ((read_bytes = read(fd1, &buf, sizeof(buf))) > 0 )
    {
        if (write(fd2, &buf, read_bytes) < 0) 
        {
            err(5, "Error writing");
        }
	}

    if (read_bytes == -1)
    {
        err(6, "Error reading");
    }

    while ((read_bytes = read(patch, &data, sizeof(data))) > 0)
    {
        if (lseek(fd2, data.offset, SEEK_SET) < 0) 
        {
            err(7, "Error with lseek");
        }

        if (read(fd2, &buf, sizeof(buf)) < 0) 
        {
            err(6, "Error with reading");
        }

        if (buf != data.originalByte) 
        {
            err(9, "Different patch file");
        }
        if (lseek(fd2, data.offset, SEEK_SET) < 0) 
        {
            err(7, "Error with lseek");
        }

        if (write(fd2, &data.newByte, sizeof(data.newByte)) < 0) 
        {
            err(5, "Error with writing");
        }
    }

    if (read_bytes == -1)
    {
        err(6, "Error reading");
    }

    close(patch);
    close(fd1);
    close(fd2);

    exit(0);
}





