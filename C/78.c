// Зад. 78 2016-SE-01 Напишете програма на C, която по подадено име на (текстови) файл като параметър, извежда съдържанието на файла сортирано, чрез употреба на външните програми cat и sort през pipe().

#include <err.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        errx(1, "Need just one argument");
    }

    int a[2];
	int b[2];

	if ( pipe(a) == -1 ) 
    {
		err(1, "a cant be piped");	
	}

	int fd = fork();
	if ( fd == -1 ) 
    {
		err(2, "fork failed");	
	}

	if ( fd == 0 ) 
    {
		close(a[0]);//a[0] - read
		dup2(a[1],1);
		if (execlp("cat", "cat", argv[1], (char*)NULL) == -1) 
        {
			err(3, "Could not exec.");
		}	
	}	
	close(a[1]);

    int status;
    wait(&status);

    if (status != 0)
    {
        errx(4, "cat failed");
    }
	
	if ( pipe(b) == -1 ) 
    {
		err(1, "b cant be piped");	
	}

	int fd2 = fork();
	if ( fd2 == -1 ) 
    {
		err(2, "fork failed");	
	}

	if ( fd2 == 0 ) {
		close(b[0]);//b[0] - read
		dup2(a[0], 0);
		dup2(b[1],1);
		if (execlp("sort", "sort", (char*)NULL) == -1) 
        {
			err(3, "Could not exec.");
		}	
	}
	close(b[1]);

    wait(&status);

    if (status != 0)
    {
        errx(5, "sort failed");
    }

    char buffer[4096];
    ssize_t bytes_read;

    while ((bytes_read = read(b[0], buffer, sizeof(buffer))) > 0)
    {
        if (write(0, buffer, bytes_read) != bytes_read)
        {
            err(6, "error writing");
        }
    }
    if (bytes_read == -1)
    {
        err(7, "error reading");
    }

    exit(0);
}