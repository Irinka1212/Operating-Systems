// T2 - Напишете програма, която приема име на команда като параметър. След това чете редове от STDIN, като за всеки ред fork-ва ново копие на подадената команда и подава ѝ реда като аргументи (разделени по спейс).

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        errx(1, "One argument expected");
    }

    char* cmd = argv[1];

    pid_t pid;
    int status;

    int line_number = 1;
    char* line = NULL;
    size_t line_size = 0;
    ssize_t read;

    int pd[2];

    if (pipe(pd) == -1) {
        err(2, "Pipe failed");
    }

    pid = fork();

    if (pid == -1) {
        err(3, "Fork failed");
    }
    if (pid == 0) {
        close(pd[1]); // Close unused write end of pipe

        dup2(pd[0], STDIN_FILENO); // Redirect stdin to pipe read end
        close(pd[0]); // Close pipe read end

        if (execlp(cmd, cmd, (char *) NULL) == -1) {
            err(4, "Error with exec: %s", cmd);
        }
        exit(0);
    }

    close(pd[0]); // Close unused read end of pipe

    while ((read = getline(&line, &line_size, stdin)) != -1) {
        line[strcspn(line, "\n")] = '\0'; // Remove trailing newline character

        write(pd[1], line, strlen(line));
        write(pd[1], " ", 1); // Add space as a separator
        line_number++;
    }

    close(pd[1]); // Close write end of pipe

    waitpid(pid, &status, 0);
    if (status != 0) {
        errx(7, "Command failed");
    }

    free(line);
    return 0;
}