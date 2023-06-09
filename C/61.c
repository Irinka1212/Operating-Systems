// Зад. 61 2017-SE-02 Напишете програма на C, която да работи подобно на командата cat, реализирайки само следната функционалност:
// • общ вид на изпълнение: ./main [OPTION] [FILE]...
// • ако е подаден като първи параметър -n, то той да се третира като опция, което кара програмата ви да номерира (глобално) всеки изходен ред (започвайки от 1).
// • програмата извежда на STDOUT
// • ако няма подадени параметри (имена на файлове), програмата чете от STDIN
// • ако има подадени параметри – файлове, програмата последователно ги извежда
// • ако някой от параметрите е тире (-), програмата да го третира като специално име за STDIN
// Примерно извикване:
// $ cat a.txt
// a1
// a2
// $ cat b.txt
// b1
// b2
// b3
// $ echo -e "s1\ns2" | ./main -n a.txt - b.txt
// 1 a1
// 2 a2
// 3 s1
// 4 s2
// 5 b1
// 6 b2
// 7 b3
// Забележка: Погледнете setbuf(3) и strcmp(3).

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdbool.h>
#include <fcntl.h>
#include <err.h>

int main(int argc, char* argv[]){
    if(argc == 1){
        char buf[4096];

        ssize_t bytes_read;

        while( (bytes_read = read(0, &buf, sizeof(buf))) > 0){
            if(write(1, &buf, bytes_read) != bytes_read){
                err(2, "Error writing");
            }
        }

            if(bytes_read == -1){
                err(1, "Error reading");
            }

            exit(0);
    }

    bool numbers = false;
    int lines = 1;
    bool newLine = true;
    int argcCnt = 1;

    setbuf(stdout, NULL);

    if(strcmp(argv[1], "-n") == 0){
        numbers = true;
        argcCnt = 2;

        if(argc == 2){
            char buf;
            ssize_t bytes_read;
            while((bytes_read = read(0, &buf, sizeof(buf))) > 0){
                if(newLine){
                    printf("%d ", lines);
                    newLine = false;
                }

                if(buf == '\n'){
                    lines++;
                    newLine = true;
                }

                if(write(1, &buf, sizeof(buf)) < 0){
                    err(2, "Error writing");
                }
            }

            if(bytes_read == -1){
                err(1, "Error reading");
            }

            exit(0);
        }
    }

    for(; argcCnt < argc; argcCnt++){
        int fd;
        if(strcmp(argv[argcCnt], "-") == 0){
            fd = 0;
        }
        else{
            fd = open(argv[argcCnt], O_RDONLY);
            if(fd < 0){
                warn("Cant open file");
                continue;
            }
        }

        char buf;
        ssize_t bytes_read;

        while((bytes_read = read(fd, &buf, sizeof(buf))) > 0){
            if(numbers && newLine){
                printf("%d ", lines);
                newLine = false;
            }

            if(numbers && buf == '\n'){
                lines++;
                newLine = true;
            }

            if(write(1, &buf, sizeof(buf)) < 0){
                err(2, "Error writing");
            }
        }

        if(bytes_read == -1){
            err(1, "Error reading");
        }

        if(fd != 0){
            close(fd);
        }
    }

    exit(0);
}