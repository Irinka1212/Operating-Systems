//T5 - Реализирайте команда cp, работеща с два аргумента, подадени като входни параметри.

#include <fcntl.h>
#include <err.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	if (argc != 3) {
		errx(1, "err");
	}

	int f11 = open(argv[1], O_RDONLY);
	if (f11 == -1) {
		err(2, "Cannot open file");
	}

	int f12 = open(argv[2], O_WRONLY | O_TRUNC);
	if (f12 == -1) {
		err(2, "Cannot open file");
	}

	char c;
	ssize_t read_bytes;
	
	while (read_bytes=(read(f11, &c, sizeof(c))) > 0 ){
		if(write(f12, &c, sizeof(c)) == -1) {
			err(3, "Err with write");
		}

	if (read_bytes == -1) {
		err(4, "Err with read");
	}

	close(f11);
	close(f12);
	exit(0);
}