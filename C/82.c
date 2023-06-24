// Зад. 82 2018-SE-01 Напишете програма на C, която приема параметър – име на директория. Програмата трябва да извежда името на най-скоро променения (по съдържание) файл в тази директория и нейните под-директории, 
// чрез употреба на външни шел команди през pipe().

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>

int main(int argc, char* argv[]) 
{
    if (argc != 2)
    {
        erx(1, "Arguments count");
    }

	int a[2];
	int b[2];
    int c[2];

	if ( pipe(a) == -1 )
    {
		err(2, "a cant be piped");	
	}

	int fd = fork();
	if ( fd == -1 )
    {
		err(3, "fork failed");	
	}

	if ( fd == 0 ) 
    {
		close(a[0]);
		dup2(a[1],1);
		if (execlp("find", "find", argv[1], "-type", "f", "-printf", "%T@ %p\n", (char*)NULL) == -1)  
        {
			err(4, "Could not exec.");
		}	
	}	
	close(a[1]);
	
	if ( pipe(b) == -1 ) 
    {
		err(2, "b cant be piped");	
	}

	int fd2 = fork();
	if ( fd2 == -1 ) 
    {
		err(3, "fork failed");	
	}

	if ( fd2 == 0 ) 
    {
		close(b[0]);
		dup2(a[0], 0);
		dup2(b[1],1);
		if (execlp("sort", "sort", "-n", (char*)NULL) == -1) 
        {
			err(4, "Could not exec.");
		}	
	}
	close(b[1]);

    if ( pipe(c) == -1 ) 
    {
		err(2, "b cant be piped");	
	}

	int fd3 = fork();
	if ( fd3 == -1 ) 
    {
		err(3, "fork failed");	
	}

	if ( fd3 == 0 ) 
    {
		close(c[0]);
		dup2(b[0], 0);
		dup2(c[1],1);
		if (execlp("tail", "tail", "-n1", (char*)NULL) == -1) 
        {
			err(4, "Could not exec.");
		}	
	}
	close(c[1]);

	dup2(c[0], 0);	
	if (execlp("awk", "awk", "{print$2}", (char*)NULL)==-1) 
    {
			err(4, "Could not exec.");
	}

	exit(0);
}