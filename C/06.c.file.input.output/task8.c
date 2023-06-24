//T8 - Koпирайте файл /etc/passwd в текущата ви работна директория и променете разделителят на копирания файл от ":", на "?"

#include <fcntl.h>
#include <err.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	int fd1;
	int output;
	int i = 0;
	char c;

	if ((fd1 = open("passwd", O_RDONLY)) == -1) {
		err(2,"Cannot open file");
	}

	if ( ( output = open("output.txt", O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR)) ) == -1 ){ 
		err(2, "Cannot open file");
    	}

	while (read(fd1, &c, sizeof(c)) == sizeof(c)) {
		if ( c == ':' ) {
			c = '?';
		}
		if (c == '\n') {
			i++;
		}

		if (write(output, &c, 1) == -1) {
            		err(4, "Error while writing to output file");
			i++;
			continue;
        	}
	}

	close(fd1);
	close(output);
	exit(0);
}