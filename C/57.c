// Зад. 57 2016-SE-02 Двоичните файлове f1 и f2 съдържат 32 битови числа без знак (uint32_t). Файлът f1 е съдържа n двойки числа, нека i-тата двойка е < xi,yi >. 
// Напишете програма на C, която извлича интервалите с начало xi и дължина yi от файла f2 и ги записва залепени в изходен файл f3.
// Пример:
// • f1 съдържа 4 числа (2 двойки): 30000, 20, 19000, 10
// • програмата записва в f3 две поредици 32-битови числа, взети от f2 както следва:
// • най-напред се записват числата, които са на позиции 30000, 30001, 30002, ... 30019.
// • след тях се записват числата от позиции 19000, 19001, ... 19009.
// Забележка: С пълен брой точки ще се оценяват решения, които работят със скорост, пропорционална на размера на изходния файл f3.

#include <err.h>
#include <fcntl.h>
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

struct pair 
{
    uint32_t x;
    uint32_t y;

}__attribute__((packed));

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        errx(1, "The arguments need to be two");
    }

    int fd1 = open(argv[1], O_RDONLY);
    if (fd1 == -1)
    {
        err(2, "Cannot open file");
    }

    int fd2 = open(argv[2], O_RDONLY);
    if (fd2 == -1)
    {
        err(2, "Cannot open file");
    }

    int fd3 = open("f3", O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
    if (fd3 == -1)
    {
        err(2, "Cannot open file");
    }

    struct pair data;
    struct stat st;

    if (fstat(fd1, &st) == -1 || fstat(fd2, &st) == -1)
    {
        err(3, "Error with fstat");
    }

    if (st.st_size % sizeof(data) != 0 || st.st_size % sizeof(uint32_t) != 0)
    {
        errx(4, "Error in files");
    }

	ssize_t read_bytes = 0;
	while ((read_bytes = read(fd1, &data, sizeof(data))) > 0)
    {
        if(lseek(fd2, data.x * sizeof(uint32_t), SEEK_SET) == -1)
        {
            err(5, "Error with lseek");
        }

		uint32_t buf;
        for(uint32_t i = 0; i < data.y; ++i)
        {
            read_bytes = read(fd2, &buf, sizeof(buf));
            if (read_bytes == -1)
            {
                err(6, "Error while reading");
            }

			if(write(fd3, &buf, sizeof(buf)) != sizeof(buf))
            {
				err(7,"Error while writing");
			}
        }
    }

    if (read_bytes == -1)
    {
        err(6, "Error while reading");
    }

    close(fd1);
    close(fd2);
    close(fd3);
    exit(0);
}