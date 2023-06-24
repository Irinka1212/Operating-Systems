// T11 - Да се напише програма на C, която изпълнява последователно подадените ѝ като параметри команди, като реализира следната функционалност постъпково: main cmd1 ... cmdN 
// Изпълнява всяка от командите в отделен дъщерен процес. ... при което се запазва броя на изпълнените команди, които са дали грешка и броя на завършилите успешно.

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {

    int count_success = 0;
    int count_failed = 0;

    for(int i = 1; i < argc; ++i)
    {	
    	char* cmd = argv[i];

	pid_t pid = fork();
    	int status;
    	if (pid == -1) 
    	{
        	err(2, "Fork failed");
    	} 

    	if (pid == 0) 
    	{
        	if (execlp(cmd, cmd, (char *) NULL) == -1) 
		{
            		err(3, "Error with exec");
        	}
    	} 
    	else 
    	{
        	waitpid(pid, &status, 0);
		if (status != 0)
        	{
			++count_failed;
            		errx(42, "Command failed: %s", cmd);
        	}

	        printf("Command executed successfully: %s\n", cmd);
		++count_success;
		
    	}
    }

    printf("Success: %d\n", count_success);
    printf("Failed: %d\n", count_failed);

    return 0;
}