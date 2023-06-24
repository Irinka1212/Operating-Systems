// Зад. 76 2022-SE-01 Напишете програма на C, която приема два позиционни аргумента – имена на двоични файлове. Примерно извикване: ./main data.bin comparator.bin
// Файлът data.bin се състои от две секции – 8 байтов хедър и данни. Структурата на хедъра е:
// • uint32_t, magic – магическа стойност 0x21796F4A, която дефинира, че файлът следва тази спецификация;
// • uint32_t, count – описва броя на елементите в секцията с данни.
// Секцията за данни се състои от елементи – uint64_t числа. Файлът comparator.bin се състои от две секции – 16 байтов хедър и данни. Структурата на хедъра е:
// • uint32_t, magic1 – магическа стойност 0xAFBC7A37;
// • uint16_t, magic2 – магическа стойност 0x1C27;
// • комбинацията от горните две magic числа дефинира, че файлът следва тази спецификация;
// • uint16_t, reserved – не се използва;
// • uint64_t, count – описва броя на елементите в секциата с данни.
// Секцията за данни се състои от елементи – наредени 6-торки:
// • uint16_t, type – възможни стойности: 0 или 1;
// • 3 бр. uint16_t, reserved – възможни стойности за всяко: 0;
// • uint32_t, offset1;
// • uint32_t, offset2.
// Двете числа offset дефинират отместване (спрямо N0) в брой елементи за data.bin; type дефинира операция за сравнение:
// • 0: “по-малко”;
// • 1: “по-голямо”.
// Елементите в comparator.bin дефинират правила от вида:
// • “елементът на offset1” трябва да е “по-малък” от “елементът на offset2”;
// • “елементът на offset1” трябва да е “по-голям” от “елементът на offset2”.
// Програмата трябва да интепретира по ред дефинираните правила в comparator.bin и ако правилото не е изпълнено, да разменя in-place елементите на съответните отмествания. 
// Елементи, които са равни, няма нужда да се разменят.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <err.h>

typedef struct {
    uint32_t magic;
    uint32_t count;
} __attribute__((packed)) HeaderData;

typedef struct {
    uint16_t type;
    uint16_t reserved[3];
    uint32_t offset1;
    uint32_t offset2;
} __attribute__((packed)) Rule;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        errx(1, "Arguments need to be two");
    }

    int data_fd = open(argv[1], O_RDWR);
    if (data_fd == -1) {
        err(2, "Cannot open");
    }

    int comparator_fd = open(argv[2], O_RDWR);
    if (comparator_fd == -1) {
        err(2, "Cannot open");
    }

    HeaderData data_header;
    HeaderData comparator_header;

    struct stat st1, st2;
    if (fstat(data_fd, &st1) != 0 || fstat(comparator_fd, &st2) != 0) {
        err(3, "Error with fstat");
    }

    if (st1.st_size % sizeof(HeaderData) != 0 || st1.st_size % sizeof(uint64_t) != 0 ||
        st2.st_size % sizeof(HeaderData) != 0 || st2.st_size % sizeof(Rule) != 0) {
        errx(4, "Error with file format");
    }

    ssize_t read_bytes;
    if ((read_bytes = read(data_fd, &data_header, sizeof(data_header))) != sizeof(data_header)) {
        err(5, "Error reading");
    }

    if (data_header.magic != 0x21796F4A) {
        errx(6, "Invalid magic value");
    }

    if ((read_bytes = read(comparator_fd, &comparator_header, sizeof(comparator_header))) != sizeof(comparator_header)) {
        err(5, "Error reading");
    }

    if (comparator_header.magic != 0xAFBC7A37) {
        errx(6, "Invalid magic value");
    }

    uint32_t data_count = data_header.count;
    uint64_t comparator_count = comparator_header.count;
    if (data_count == 0 || comparator_count == 0) {
        errx(7, "empty files");
    }

    size_t data_element_size = sizeof(uint64_t);
    size_t comparator_element_size = sizeof(Rule);

    Rule rule;
    for (uint64_t i = 0; i < comparator_count; ++i) {
        if (read(comparator_fd, &rule, comparator_element_size) != comparator_element_size) {
            err(5, "Error reading");
        }

        off_t offset1 = rule.offset1 * data_element_size;
        off_t offset2 = rule.offset2 * data_element_size;

        if (rule.type == 0) {
            uint64_t value1, value2;
            if (lseek(data_fd, offset1, SEEK_SET) == -1) {
                err(8, "Error with lseek");
            }
            if (read(data_fd, &value1, data_element_size) != data_element_size) {
                err(5, "Error reading");
            }

            if (lseek(data_fd, offset2, SEEK_SET) == -1) {
                err(8, "Error with lseek");
            }
            if (read(data_fd, &value2, data_element_size) != data_element_size) {
                err(5, "Error reading");
            }

            if (value1 > value2) {
                if (lseek(data_fd, offset1, SEEK_SET) == -1) {
                    err(8, "Error with lseek");
                }
                if (write(data_fd, &value2, data_element_size) != data_element_size) {
                    err(9, "Error writing");
                }

                if (lseek(data_fd, offset2, SEEK_SET) == -1) {
                    err(8, "Error with lseek");
                }
                if (write(data_fd, &value1, data_element_size) != data_element_size) {
                    err(9, "Error writing");
                }
            }

        } else if (rule.type == 1) {
            uint64_t value1, value2;
            if (lseek(data_fd, offset1, SEEK_SET) == -1) {
                err(8, "Error with lseek");
            }
            if (read(data_fd, &value1, data_element_size) != data_element_size) {
                err(5, "Error reading");
            }

            if (lseek(data_fd, offset2, SEEK_SET) == -1) {
                err(8, "Error with lseek");
            }
            if (read(data_fd, &value2, data_element_size) != data_element_size) {
                err(5, "Error reading");
            }

            if (value1 < value2) {
                if (lseek(data_fd, offset1, SEEK_SET) == -1) {
                    err(8, "Error with lseek");
                }
                if (write(data_fd, &value2, data_element_size) != data_element_size) {
                    err(9, "Error writing");
                }

                if (lseek(data_fd, offset2, SEEK_SET) == -1) {
                    err(8, "Error with lseek");
                }
                if (write(data_fd, &value1, data_element_size) != data_element_size) {
                    err(9, "Error writing");
                }
            }

        } else {
            errx(10, "Invalid rule type");
        }
    }

    close(data_fd);
    close(comparator_fd);
    exit(0);
}