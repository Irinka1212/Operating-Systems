// Зад. 80 2017-IN-01 Напишете програма на C, която използвайки външни shell команди през pipe() да извежда статистика за броя на използване на различните shell-ове от потребителите, дефинирани в системата. 
// Изходът да бъде сортиран във възходящ ред според брой използвания на shell-овете. Примерно извикване и изход: $ ./main
// 1 /bin/sync
// 3 /bin/bash
// 7 /bin/false
// 17 /usr/sbin/nologin

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>

int main(void) 
{
	int a[2];
	int b[2];
    int c[2];

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
		if (execlp("cut", "cut", "-d:", "-f7", "/etc/passwd",(char*)NULL) == -1) 
        {
			err(3, "Could not exec.");
		}	
	}	
	close(a[1]);
	
	if ( pipe(b) == -1 ) 
    {
		err(1, "b cant be piped");	
	}

	int fd2 = fork();
	if ( fd2 == -1 ) 
    {
		err(2, "fork failed");	
	}

	if ( fd2 == 0 ) 
    {
		close(b[0]);
		dup2(a[0], 0);
		dup2(b[1],1);
		if (execlp("sort", "sort", (char*)NULL) == -1) 
        {
			err(3, "Could not exec.");
		}	
	}
	close(b[1]);

    if ( pipe(c) == -1 ) 
    {
		err(1, "b cant be piped");	
	}

	int fd3 = fork();
	if ( fd3 == -1 ) 
    {
		err(2, "fork failed");	
	}

	if ( fd3 == 0 ) 
    {
		close(c[0]);
		dup2(b[0], 0);
		dup2(c[1],1);
		if (execlp("uniq", "uniq", "-c", (char*)NULL) == -1) 
        {
			err(3, "Could not exec.");
		}	
	}
	close(c[1]);

	dup2(c[0], 0);	
	if (execlp("sort", "sort", "-n", (char*)NULL)==-1) 
    {
			err(3, "Could not exec.");
	}

	exit(0);
}