//T4 - cat /etc/passwd | sort | uniq

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>

int main(int argc, char* argv[]) {
	int a[2];
	int b[2];
	char c;

	if ( pipe(a) == -1 ) {
		err(1, "a cant be piped");	
	}

	int fd = fork();
	if ( fd == -1 ) {
		err(2, "fork failed");	
	}

	if ( fd == 0 ) {
		close(a[0]);//a[0] - read
		dup2(a[1],1);
		if (execlp("cat", "cat", "/etc/passwd", (char*)NULL) == -1) {
			err(3, "Could not exec.");
		}	
	}	
	close(a[1]);
	
	if ( pipe(b) == -1 ) {
		err(1, "b cant be piped");	
	}

	int fd2 = fork();
	if ( fd2 == -1 ) {
		err(2, "fork failed");	
	}

	if ( fd2 == 0 ) {
		close(b[0]);//a[0] - read
		dup2(a[0], 0);
		dup2(b[1],1);
		if (execlp("sort", "sort", (char*)NULL) == -1) {
			err(3, "Could not exec.");
		}	
	}
	close(b[1]);

	dup2(b[0], 0);	
	if (execlp("uniq", "uniq", (char*)NULL) == -1) {
			err(3, "Could not exec.");
	}

	exit(0);
}