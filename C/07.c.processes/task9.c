// T9 - Да се напише програма на C, която която създава файл в текущата директория и генерира два процесa, които записват низовете foo и bar в създадения файл. 
// Програмата не гарантира последователното записване на низове. Променете програмата така, че да записва низовете последователно, като първия е foo.

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(void)
{
	int fd = open("file.txt", O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
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
        	const char* data = "foo";
        	ssize_t bytes_written = write(fd, data, strlen(data));
        	if (bytes_written == -1)
        	{
            		err(4, "Error while writing");
        	}

		exit(0);
    	} 
    	else 
    	{
		int status;
       	 	waitpid(pid, &status, 0);
        	if (status != 0)
        	{
            		err(5, "Child process failed");
        	}

    	}

	pid = fork();
	if(pid == -1)
	{
		err(3,"Error with fork");
	}

	if (pid == 0) 
    	{
        	const char* data = "bar";
        	ssize_t bytes_written = write(fd, data, strlen(data));
        	if (bytes_written == -1)
        	{
            		err(4, "Error while writing");
        	}
		exit(0);
    	} 
    	else 
    	{
		int status;
       	 	waitpid(pid, &status, 0);
        	if (status != 0)
        	{
            		err(5, "Child process failed");
        	}

    	}

	close(fd);
	return 0;
}
