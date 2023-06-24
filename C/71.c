// Зад. 71 2020-SE-02 Инженерите от съседната лабораторя работят с комплекти SCL/SDL файлове, напр. input.scl/input.sdl. В SCL файла са записани нива на сигнали (ниско 0 или високо 1), 
// т.е., файлът се третира като състоящ се от битове. В SDL файла са записани данни от тип uint16_t, като всеки елемент съответства позиционно на даден бит от SCL файла. 
// Помогнете на колегите си, като напишете програма на C, която да бъде удобен инструмент за изваждане в нов файл само на тези SDL елементи, които са имали високо ниво в SCL файла, запазвайки наредбата им.

#include <stdlib.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>

int main(int arc, char * argv[])
{
    if (argc != 3)
    {
        errx(2, "not two arguments")
    }
    int scl = open(argv[1], O_RDONLY);
    if (scl == -1)
    {
        err(1, "Cannot open file");
    }
    int sdl = open(argv[2], O_RDONLY);
    if (sdl == -1)
    {
        err(1, "Cannot open file");
    }

    int output = open("output.bin", O_CREAT | O_RDWR | O_TRUNC | S_IRWXU);
    if (output == -1)
    {
        err(1, "Cannot open file");
    }

    uint8_t buf;
    uint16_t readFromSDL;
    uint16_t count = 0;
    while(read(scl,&buf,sizeof(buf)) > 0)
    {
        for(int i = 7; i >= 0; --i)
        {
            if(buf & (1<<i) != 0)
            {
                lseek(sdl, (count)*sizeof(uint16_t), SEEK_SET);
                if(read(sdl,&readFromSDL,sizeof(readFromSDL)) != sizeof(readFromSDL))
                {
                    err(2, "Error while read");
                }

                if(write(output, &readFromSDL, sizeof(readFromSDL) != sizeof(readFromSDL)))
                {
                    err(3, "Error while write");
                }
            }
            count++;
        }
    }

    close(scl);
    close(sdl);
    close(output);
    return 0;
}