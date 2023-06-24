//T3 - Реализирайте команда wc, с един аргумент подаден като входен параметър

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

int main (int argc, char* argv[]){
	int fd1;
	char c;
	int lines = 0;
	int words = 0;
	int chars = 0;

	if (argc != 2) {
		write(2, "err\n", 4);
		exit(1);
	}

	if ( (fd1 = open(argv[1], O_RDONLY)) == -1 ) {
		write(2, "File failed to open in read mode\n", 33);
		exit(1);
	}

	while ( read(fd1, &c, sizeof(c)) == sizeof(c) ) {
		if (c=='\n') {
			lines++;
			words++;
		}

		if (c==' ') {
			words++;
		}

		chars++;
	}

	printf("File %s has:\n%d number of lines.\n%d number of words.\n%d number of chars.\n",
		argv[1], lines, words, chars);
		
	close(fd1);
	exit(0);
}