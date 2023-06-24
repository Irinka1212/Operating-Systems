// T1 - Напишете програма, която приема като аргументи имена на две команди, и след това прочита редове от STDIN. Четните редове се пращат на STDIN на първата команда, а нечетните - на STDIN на втората. 
// Програмата излиза, когато и двете команди излязат.

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        errx(1, "Two arguments expected");
    }

    char* first_cmd = argv[1];
    char* second_cmd = argv[2];

    pid_t pid1, pid2;
    int status1, status2;

    int line_number = 1;
    char* line = NULL;
    size_t line_size = 0;
    ssize_t read;

    int pipe1[2];
    int pipe2[2];

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        err(2, "Pipe failed");
    }

    pid1 = fork();

    if (pid1 == -1) {
        err(3, "Fork failed");
    }
    if (pid1 == 0) {
        close(pipe1[1]);  // Close unused write end of pipe1
        close(pipe2[0]);  // Close unused read end of pipe2

        dup2(pipe1[0], STDIN_FILENO);  // Redirect stdin to pipe1 read end
        close(pipe1[0]);  // Close pipe1 read end
        close(pipe2[1]);  // Close pipe2 write end

        if (execlp(first_cmd, first_cmd, (char *) NULL) == -1) {
            err(4, "Error with exec: %s", first_cmd);
        }
        exit(0);
    }

    pid2 = fork();
    if (pid2 == -1) {
        err(5, "Fork failed");
    }
    if (pid2 == 0) {
        close(pipe1[0]);  // Close unused read end of pipe1
        close(pipe2[1]);  // Close unused write end of pipe2

        dup2(pipe2[0], STDIN_FILENO);  // Redirect stdin to pipe2 read end
        close(pipe2[0]);  // Close pipe2 read end
        close(pipe1[1]);  // Close pipe1 write end

        if (execlp(second_cmd, second_cmd, (char *) NULL) == -1) {
            err(6, "Error with exec: %s", second_cmd);
        }
        exit(0);
    }

    close(pipe1[0]);  // Close unused read end of pipe1
    close(pipe2[0]);  // Close unused read end of pipe2

    while ((read = getline(&line, &line_size, stdin)) != -1) {
        if (line_number % 2 == 0) {
            write(pipe1[1], line, read);
        } else {
            write(pipe2[1], line, read);
        }
        line_number++;
    }

    close(pipe1[1]);  // Close write end of pipe1
    close(pipe2[1]);  // Close write end of pipe2

    waitpid(pid1, &status1, 0);
    if (status1 != 0) {
        errx(7, "Command 1 failed");
    }

    waitpid(pid2, &status2, 0);
    if (status2 != 0) {
        errx(8, "Command 2 failed");
    }

    free(line);
    return 0;
}