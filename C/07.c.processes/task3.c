//T3 - Да се напише програма на C, която изпълнява команда sleep (за 60 секунди).

#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <stdio.h>

int main(void)
{
	if (execl("/bin/sleep", "pesho", "60", 0) == -1) {
		err(99, "err execling");
	} 

	exit(0);
}