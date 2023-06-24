//T10 - Да се напише програма на C, която получава като параметри от команден ред две команди (без параметри). Изпълнява първата. Ако тя е завършила успешно изпълнява втората. Ако не, завършва с код 42.

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        errx(1, "Not two arguments");
    }

    char* first_cmd = argv[1];
    char* second_cmd = argv[2];

    pid_t pid = fork();
    int status;
    if (pid == -1) 
    {
        err(2, "Fork failed");
    } 

    if (pid == 0) 
    {
        if (execlp(first_cmd, first_cmd, (char *) NULL) == -1) 
	{
            err(3, "Error with exec");
        }
    } 
    else 
    {
        waitpid(pid, &status, 0);
	if (status != 0)
        {
            	errx(42, "Command failed: %s", first_cmd);
        }
        printf("Command executed successfully: %s\n", first_cmd);
    }

    pid = fork();
    if (pid == -1) 
    {
        err(2, "Fork failed");
    } 

    if (pid == 0) 
    {
        if (execlp(second_cmd, second_cmd, (char *) NULL) == -1) 
	{
            err(3, "Error with exec");
        }
    } 
    else 
    {
        waitpid(pid, &status, 0);
	if (status != 0)
        {
            	errx(42, "Command failed: %s", second_cmd);
        }
        printf("Command executed successfully: %s\n", second_cmd);
    }

    return 0;
}