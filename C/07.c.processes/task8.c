// T8 - Да се напише програма на С, която получава като параметър име на файл. Създава процес син, който записва стринга foobar във файла (ако не съществува, го създава, в противен случай го занулява), 
// след което процеса родител прочита записаното във файла съдържание и го извежда на стандартния изход, добавяйки по един интервал между всеки два символа.

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		errx(1, "Not one argument");
	}

	int fd = open(argv[1], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		err(2, "Cannot open file");
	}

	pid_t pid = fork();
	if(pid == -1)
	{
		err(3,"Error with fork");
	}

	if (pid == 0) 
    	{
        	const char* data = "foobar";
        	ssize_t bytes_written = write(fd, data, strlen(data));
        	if (bytes_written == -1)
        	{
            		err(4, "Error while writing");
        	}
    	} 
    	else 
    	{
		int status;
       	 	waitpid(pid, &status, 0);
        	if (status != 0)
        	{
            		err(5, "Child process failed");
        	}

        	lseek(fd, 0, SEEK_SET);

        	char c;
        	int count = 0;
        	while (read(fd, &c, sizeof(c)) == sizeof(c))
        	{
            		if (write(1, &c, sizeof(c)) == -1)
            		{
                		err(4, "Error while writing");
            		}

            		++count;
            		if (count == 2)
            		{
                		write(1, " ", 1);
                		count = 0;
            		}
        	}
    	}

	close(fd);
	return 0;
}
