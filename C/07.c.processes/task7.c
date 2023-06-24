//T7 - Да се напише програма на С, която получава като параметри три команди (без параметри), изпълнява ги последователно, 
//като изчаква края на всяка и извежда на стандартния изход номера на завършилия процес, както и неговия код на завършване.

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        errx(1, "Not three arguments");
    }

    char* first_cmd = argv[1];
    char* second_cmd = argv[2];
    char* third_cmd = argv[3];

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
        printf("Command executed successfully: %s\n", second_cmd);
    }

    pid = fork();
    if (pid == -1) 
    {
        err(2, "Fork failed");
    } 

    if (pid == 0) 
    {
        if (execlp(third_cmd, third_cmd, (char *) NULL) == -1) 
	{
            err(3, "Error with exec");
        }
    } 
    else 
    {
        waitpid(pid, &status, 0);
        printf("Command executed successfully: %s\n", third_cmd);
    }

    return 0;
}