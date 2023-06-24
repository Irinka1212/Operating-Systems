// T13 - Да се напише програма на C, която получава като командни параметри две команди (без параметри). Изпълнява ги едновременно и извежда на стандартния изход номера на процеса на първата завършила успешно. 
// Ако нито една не завърши успешно извежда -1.

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
        errx(1, "Not two arguments");
    }

    char* first_cmd = argv[1];
    char* second_cmd = argv[2];
    
    pid_t pid_first = fork();
    int status_first;
    if (pid_first == -1) {
        err(2, "Fork failed");
    } 

    if (pid_first == 0) {
        if (execlp(first_cmd, first_cmd, (char *) NULL) == -1) {
            err(3, "Error with exec");
        }
    } 

    pid_t pid_second = fork();
    int status_second;
    if (pid_second == -1) {
        err(4, "Fork failed");
    } 

    if (pid_second == 0) {
        if (execlp(second_cmd, second_cmd, (char *) NULL) == -1) {
            err(5, "Error with exec");
        }
    } 

    pid_t completed_pid = waitpid(-1, &status_first, 0);
    if (WIFEXITED(status_first) && WEXITSTATUS(status_first) == 0) {
        printf("Command executed successfully: %s\n", first_cmd);
        printf("Process number of the first command that completed successfully: %d\n", completed_pid);
        return 0;
    }

    completed_pid = waitpid(-1, &status_second, 0);
    if (WIFEXITED(status_second) && WEXITSTATUS(status_second) == 0) {
        printf("Command executed successfully: %s\n", second_cmd);
        printf("Process number of the first command that completed successfully: %d\n", completed_pid);
        return 0;
    }

    printf("Neither command completed successfully.\n");
    printf("Process number of the first command that completed successfully: -1\n");

    return 0;
}