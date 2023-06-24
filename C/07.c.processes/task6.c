//T6 - Да се напише програма на С, която получава като параметър команда (без параметри) и при успешното ѝ изпълнение, извежда на стандартния изход името на командата.

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        errx(1, "Need to provide only one argument");
    }

    char* cmd = argv[1];

    pid_t pid = fork();
    if (pid < 0) {
        err(2, "Fork failed");
    } else if (pid == 0) {
        // Child process
        if (execlp(cmd, cmd, (char *) NULL) == -1) {
            err(3, "Error with exec");
        }
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        printf("Command executed successfully: %s\n", cmd);
    }

    return 0;
}