//T10 - Модифицирайте предната програма, така че да намерите най-големия байт.
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdlib.h>

void find_largest_byte(const char* file_path) {
    uint8_t largest_byte = 0;

    int file = open(file_path, O_RDONLY);
    if (file == -1) {
        err(2, "Cannot open file");
    }

    uint8_t byte;
    ssize_t bytes_read;
    while ((bytes_read = read(file, &byte, sizeof(uint8_t))) > 0) {
        if (byte > largest_byte) {
            largest_byte = byte;
        }
    }

    close(file);

    dprintf(1, "Largest byte: %u\n", largest_byte);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        errx(1, "err");
    }

    const char* file_path = argv[1];

    find_largest_byte(file_path);

    exit(0);
}