// T9 - Напишете програма, която приема точно 2 аргумента. 
// Първият може да бъде само --min, --max или --print (вижте man 3 strcmp). 
// Вторият аргумент е двоичен файл, в който има записани цели неотрицателни двубайтови числа 
// (uint16_t - вижте man stdint.h). Ако първият аргумент е:
// --min - програмата отпечатва кое е най-малкото число в двоичния файл.
// --max - програмата отпечатва кое е най-голямото число в двоичния файл.
// --print - програмата отпечатва на нов ред всяко число.
// Използвайте двоичния файл binary/dump или генерирайте сами такъв (правилно подравнен).

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdlib.h>

void print_min(uint16_t* data, size_t size) {
    uint16_t min = data[0];
    for (size_t i = 1; i < size; ++i) {
        if (data[i] < min) {
            min = data[i];
        }
    }
    dprintf(1, "Minimum number: %u\n", min);
}

void print_max(uint16_t* data, size_t size) {
    uint16_t max = data[0];
    for (size_t i = 1; i < size; ++i) {
        if (data[i] > max) {
            max = data[i];
        }
    }
    dprintf(1, "Maximum number: %u\n", max);
}

void print_numbers(uint16_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        dprintf(1, "%u\n", data[i]);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
	errx(1, "err");
    }

    const char* operation = argv[1];
    const char* file_path = argv[2];

    uint16_t data[100];
    size_t count = 0;

    int file = open(file_path, O_RDONLY);
    if (file == -1) {
            err(2, "Cannot open file");
    }

    count = read(file, data, sizeof(uint16_t) * 100);
    close(file);

    if (strcmp(operation, "--min") == 0) {
        print_min(data, count / sizeof(uint16_t));
    } else if (strcmp(operation, "--max") == 0) {
        print_max(data, count / sizeof(uint16_t));
    } else if (strcmp(operation, "--print") == 0) {
        print_numbers(data, count / sizeof(uint16_t));
    } else {
        err(3, "Another err");
    }

    exit(0);
}
