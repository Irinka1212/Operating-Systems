// T3 - Напишете програма на C, която демонстрира употребата на dup/dup2 между parent и child процеси. 
// Parent-ът трябва да изпраща стринга, получен като първи аргумент на командния ред към child-а, където той да може да се чете от stdin. Child процесът да изпълнява wc -c.

#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(const int argc, const char* const argv[]) {

	if (argc != 2) {
		errx(1, "Invalid number of arguments. Usage: %s <string>", argv[0]);
	}

	int pf[2];

	if (pipe(pf) == -1) {
		err(1, "Could not create pipe");
	}

	const pid_t child_pid = fork();
	if (child_pid == -1) {
		err(1, "Could not fork.");
	}

	if (child_pid == 0) {
		close(pf[1]);
		close(0);
		dup(pf[0]);
//		sleep(5);
		if (execlp("wc", "wc", "-c", NULL) == -1) {
			err(1, "Could not exec.");
		}
	}

	close(pf[0]);
	write(pf[1], argv[1], strlen(argv[1]));
	close(pf[1]);
	wait(NULL);

	exit(0);
}