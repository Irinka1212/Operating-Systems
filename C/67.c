// Зад. 67 2018-SE-04 Напишете програма на C, която приема два параметъра – имена на файлове:
// • примерно извикване: ./main input.bin output.bin
// • файловете input.bin и output.bin се третират като двоични файлове, състоящи се от uint16_t числа
// • файлът input.bin може да съдържа максимум 65535 числа
// • файлът output.bin трябва да бъде създаден от програмата и да съдържа числата от input.bin, сортирани във възходящ ред
// • endianness-ът на машината, създала файла input.bin е същият, като на текущата машина
// • ограничения на ресурси: програмата трябва да работи с употреба на максимум 256 KB RAM и 2 MB дисково пространство.

#include <err.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

int compareUint16_t(const void* a, const void* b)
{
    uint16_t num1 = *(const uint16_t*)a;
    uint16_t num2 = *(const uint16_t*)b;

    if (num1 < num2) return -1;
    if (num1 > num2) return 1;
    return 0;
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        errx(1, "Need two arguments");
    }

    int input = open(argv[1], O_RDONLY);
    if (input == -1)
    {
        err(2, "Cannot open");
    }

    int output = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
    if (output == -1)
    {
        err(2, "Cannot open");
    }

    struct stat st;

    if (fstat(input, &st) != 0)
    {
        err(3, "error with fstat");
    }

    if (st.st_size % sizeof(uint16_t) != 0)
    {
        errx(4, "Error with file format");
    }

    int fileSize = st.st_size;
    int numsCount = fileSize / sizeof(uint16_t);
    int maxN = 65535;
    if (numsCount > maxN)
    {
        errx(5, "Need max 65535 nums");
    }

    uint16_t* nums = malloc(numsCount * sizeof(uint16_t));
    if (numbers == NULL) 
    {
        err(6, "Error with malloc");
    }

    ssize_t read_bytes = read(input, numbers, fileSize);
    if (read_bytes == -1)
    {
        err(7, "Error reading");
    }

    qsort(numbers, numCount, sizeof(uint16_t), compareUint16_t);

    ssize_t write_bytes = write(output, numbers, fileSize);
    if (write_bytes == -1) 
    {
        err(8, "Error while writing");
    }

    close(input);
    close(output);
    free(nums);
    exit(0);
}