// Зад. 84 2020-SE-01 Напишете две програми на C (foo и bar), които си комуникират през наименована тръба. 
// Програмата foo приема параметър - име на файл, програмата bar приема параметър - команда като абсолютен път до изпълним файл. Примерни извиквания и ред на изпълнение (в отделни терминали):
// ./foo a.txt
// ./bar /usr/bin/sort
// Програмата foo трябва да изпълнява външна команда cat с аргумент името на подадения файл, така че съдържанието му да се прехвърли през тръбата към програмата bar, 
// която от своя страна трябва да изпълни подадената и като аргумент команда (без параметри; /usr/bin/sort в примера),
// която да обработи получените през тръбата данни, четейки от стандартен вход. Еквивалент на горния пример би било следното изпълнение: cat a.txt | /usr/bin/sort

#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define PIPENAME "baz.pipe"

int main(const int argc, const char* const argv[]) {
        if (argc != 2) {
                errx(1,"Usage: %s <command>", argv[0]);
        }

    struct stat sb;
    int str = stat(PIPENAME, &sb);

        if ((str == -1) && (errno == ENOENT)){
        if (mkfifo(PIPENAME, S_IRUSR | S_IWUSR) == -1) {
            err(2, "Cannot mkfifo() from %s", argv[0]);
        }

    } else if (str == -1) {
        err(3, "General stat() failure");
    }

        int fd = open(PIPENAME, O_RDONLY);
        if (fd == -1) {
                err(4, "Cannot open pipe %s from %s", PIPENAME, argv[0]);
        }

        if (dup2(fd, 0) == -1) {
                err(5, "Cannot dup fd from %d to 0", fd);
        }

        if (execl(argv[1], argv[1], (char*) NULL) == -1) {
                err(6, "Cannot exec from %s", argv[0]);
        }
        exit(0);
}
