// Зад. 69 2020-IN-01 Напишете програма на C, която приема три параметъра – имена на двоични файлове. Примерно извикване: $ ./main patch.bin f1.bin f2.bin
// Файловете patch.bin и f1.bin съществуват, и на тяхна база програмата трябва да създаде f2.bin. Файлът patch.bin се състои от две секции – 16 байтов хедър и данни. 
// На базата на хедъра програмата трябва да може да интерпретира съдържанието на файла. Структурата на хедъра е:
// • uint32_t, magic – магическа стойност 0xEFBEADDE, която дефинира, че файлът следва тази спецификация
// • uint8_t, header version – версия на хедъра, с единствена допустима стойност за момента 0x01, която дефинира останалите байтове от хедъра както следва: 
// – uint8_t, data version – версия (описание) на използваните структури в секцията за данни на файла
// – uint16_t, count – брой записи в секцията за данни
// – uint32_t, reserved 1 – не се използва
// – uint32_t, reserved 2 – не се използва
// Възможни структури в секцията за данни на файла спрямо data version:
// • при версия 0x00
// – uint16_t, offset
// – uint8_t, original byte
// – uint8_t, new byte
// • при версия 0x01
// – uint32_t, offset
// – uint16_t, original word
// – uint16_t, new word
// • забележка: и при двете описани версии offset е отместване в брой елементи спрямо началото на файла
// Двоичните файлове f1.bin и f2.bin се третират като състоящи се от елементи спрямо data version в patch.bin.
// Програмата да създава файла f2.bin като копие на файла f1.bin, но с отразени промени на базата на файла patch.bin, при следния алгоритъм:
// • за всяка наредена тройка от секцията за данни на patch.bin, ако на съответният offset в оригиналния файл f1.bin е записан елементът original byte/word, в изходният файл се записва new byte/word. 
// Ако не е записан такъв елемент или той не съществува, програмата да прекратява изпълнението си по подходящ начин;
// • всички останали елементи се копират директно.
// Наредените тройки в секцията за данни на файла patch.bin да се обработват последователно. Обърнете внимание на обработката за грешки и съобщенията към потребителя 
// – искаме програмата да бъде удобен и валиден инструмент.

#include <err.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct header 
{
    uint32_t magic;
    uint8_t headerVersion;
    uint8_t dataVersion;
    uint16_t count;
    uint32_t reserved1;
    uint32_t reserved2;

} __attribute__((packed)) Header;

typedef struct data00 
{
    uint16_t offset;
    uint8_t originalByte;
    uint8_t newByte;

} __attribute__((packed)) Data0X00;

typedef struct data01 
{
    uint32_t offset;
    uint16_t originalWord;
    uint16_t newWord;

} __attribute__((packed)) Data0X01;

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

    Header h;
    ssize_t read_bytes = read(patch, &h, sizeof(h));
    if (read_bytes == -1 || read_bytes != sizeof(h)) 
    {
        err(3, "Error reading header");
    }

    if (h.magic != 0xEFBEADDE || h.headerVersion != 0x01) 
    {
        errx(3, "Invalid header");
    }

    if (h.dataVersion == 0x00) 
    {
        Data0X00 d;
        struct stat st1, st2;

        if (fstat(patch, &st1) == -1 || fstat(fd1, &st2) == -1) 
        {
            err(4, "Error with fstat");
        }

        if (st2.st_size % sizeof(Data0X00) != 0) 
        {
            errx(5, "Error with file format");
        }

        uint8_t buf;

        while ((read_bytes = read(fd1, &buf, sizeof(buf))) > 0) 
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

        while ((read_bytes = read(patch, &d, sizeof(d))) > 0) 
        {
            if (lseek(fd2, d.offset, SEEK_SET) < 0) 
            {
                err(7, "Error with lseek");
            }

            if (read(fd2, &buf, sizeof(buf)) < 0) 
            {
                err(6, "Error with reading");
            }

            if (buf != d.originalByte) 
            {
                err(9, "Different patch file");
            }
            if (lseek(fd2, d.offset, SEEK_SET) < 0) 
            {
                err(7, "Error with lseek");
            }

            if (write(fd2, &d.newByte, sizeof(d.newByte)) < 0) 
            {
                err(5, "Error with writing");
            }
        }

        if (read_bytes == -1) 
        {
            err(6, "Error reading");
        }
    } else if (h.dataVersion == 0x01) 
    {
        Data0X01 d;
        struct stat st1, st2;

        if (fstat(patch, &st1) == -1 || fstat(fd1, &st2) == -1) 
        {
            err(4, "Error with fstat");
        }

        if (st2.st_size % sizeof(Data0X01) != 0) 
        {
            errx(5, "Error with file format");
        }

        uint8_t buf;

        while ((read_bytes = read(fd1, &buf, sizeof(buf))) > 0) 
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

        while ((read_bytes = read(patch, &d, sizeof(d))) > 0) 
        {
            if (lseek(fd2, d.offset, SEEK_SET) < 0) 
            {
                err(7, "Error with lseek");
            }

            if (read(fd2, &buf, sizeof(buf)) < 0) 
            {
                err(6, "Error with reading");
            }

            if (buf != d.originalWord) 
            {
                err(9, "Different patch file");
            }
            if (lseek(fd2, d.offset, SEEK_SET) < 0) 
            {
                err(7, "Error with lseek");
            }

            if (write(fd2, &d.newWord, sizeof(d.newWord)) < 0) 
            {
                err(5, "Error with writing");
            }
        }

        if (read_bytes == -1) 
        {
            err(6, "Error reading");
        }

    } else 
    {
        errx(3, "Invalid data version");
    }

    close(patch);
    close(fd1);
    close(fd2);

    exit(0);
}