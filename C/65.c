// Зад. 65 2018-SE-02 Напишете програма на C, която приема два параметъра – имена на файлове:
// • примерно извикване: ./main input.bin output.bin
// • файловете input.bin и output.bin се третират като двоични файлове, състоящи се от uint32_t числа
// • файлът input.bin може да съдържа максимум 4194304 числа
// • файлът output.bin трябва да бъде създаден от програмата и да съдържа числата от input.bin, сортирани във възходящ ред
// • endianness-ът на машината, създала файла input.bin е същият, като на текущата машина
// • ограничения на ресурси: програмата трябва да работи с употреба на максимум 9 MB RAM и 64 MB дисково пространство.

#include <err.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int compareUint32(const void* a, const void* b) 
{
    uint32_t num1 = *(const uint32_t*)a;
    uint32_t num2 = *(const uint32_t*)b;

    if (num1 < num2) return -1;
    if (num1 > num2) return 1;
    return 0;
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        errx(1, "Need 2 arguments");
    }

    int input = open(argv[1], O_RDONLY);
    if (input == -1)
    {
        err(2, "Cannot open file");
    }

    int output = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
    if (output == -1)
    {
        err(2, "Cannot open file");
    }

    struct stat st;
    if(fstat(input, &st) != 0)
    {
        err(3, "Error with fstat");
    }

    if (st.st_size % sizeof(uint32_t) != 0)
    {
        errx(4, "Error with file format");
    }

    
    off_t fileSize = st.st_size;
    off_t numCount = fileSize / sizeof(uint32_t);
    int maxN = 4194304;
    if (numCount > maxN)
    {
        errx(5, "Input has more numbers than it should");
    }

    uint32_t* numbers = malloc(numCount * sizeof(uint32_t));
    if (numbers == NULL) 
    {
        err(6, "Memory allocation failed");
    }

    ssize_t read_bytes = read(input, numbers, fileSize);
    if (read_bytes == -1) 
    {
        err(7, "Error while reading");
    }
 
    qsort(numbers, numCount, sizeof(uint32_t), compareUint32);

    ssize_t write_bytes = write(output, numbers, fileSize);
    if (write_bytes == -1) 
    {
        err(8, "Error while writing");
    }

    close(input);
    close(output);
    free(numbers);
    exit(0);
}