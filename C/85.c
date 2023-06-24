// Зад. 85 2020-SE-02 При изграждане на система за пренасяне на сериен асинхронен сигнал върху радиопреносна мрежа се оказало, че големи поредици от битове само нули или само единици смущават сигнала, 
// поради нестабилно ниво. Инженерите решили проблема, като:
// • в моментите, в които няма сигнал от серийният порт, вкарвали изкуствено байт 0x55 в потока;
// • реалните байтове 0x00, 0xFF, 0x55 и 0x7D се кодирали посредством XOR-ване (побитова обработка с изключващо-или) с 0x20, като полученият байт се изпращал през потока, 
// предхождан от 0x7D, който играе ролята на escape character.
// Разполагате със запис от такъв поток. Напишете програма на C, която приема два параметъра - имена на файлове. Примерно извикване: $ ./main input.lfld output.bin
// Програмата трябва да обработва записа и да генерира output.bin, който да съдържа оригиналните данни. Четенето на входните данни трябва да става посредством изпълнение на външна shell команда.

#include <stdlib.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>

int main(int argc, char * argv[])
{
    if(argc != 3)
    {
        errx(1, "wrong number of arguments");
    }

    int pd[2];
    if(pipe(pd) == -1)
    {
        err(2, "error with pipe");
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        err(3, "error while forking");
    }

    if (pid == 0)
    {
        close(pd[0]);

        dup2(pd[1], 1);
        execlp("cat", "cat", argv[1], NULL);
    }

    close(pd[1]);
    int fd = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC | S_IRWXU);
    uint8_t buf;
    while (read(pd[0], &buf, sizeof(buf)) > 0)
    {
        if(buf == 0x55)
        {
            continue;
        }

        if (buf == 0x7D)
        {
            if(read(pd[0], &buf, sizeof(buf)) != sizeof(buf))
            {
                err(3, "error while read");
            }

            uint8_t temp = buf ^ 0x20;
            if (temp != 0x00 && temp != 0x55 && temp != 0xFF && temp != 0x7D)
            {
                errx(4, "wrong file format");
            }

            write(fd, &temp, sizeof(temp));
        }
        else
        {
            write(fd, &buf, sizeof(buf));
        }
    }

    close(fd);
    close(pd[0]);
    return 0;
}