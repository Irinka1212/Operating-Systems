//with one argument

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	int fd1;
	int fd2;
	char c;

	if (argc != 2) {
		write(2, "err\n", 4);
		exit(1);
	}

	if ((fd1 = open(argv[1], O_RDONLY)) == -1) {
		write(2, "File failed to open in read mode\n", 33);
		exit(1);
	}

	if ( ( fd2 = open("bar.txt", O_CREAT|O_WRONLY) ) == -1 ){ 
		write(2, "File failed to open in write mode\n", 34);
		exit(1);
	}

	while ( read(fd1, &c, 1) == 1 ){
		write(fd2, &c, 1);
	}

	close(fd1);
	close(fd2);
	exit(0);
}