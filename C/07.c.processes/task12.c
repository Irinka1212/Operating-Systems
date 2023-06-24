// T12 - Да се напише програма на C, която получава като параметри от команден ред две команди (без параметри) и име на файл в текущата директория. Ако файлът не съществува, го създава. 
// Програмата изпълнява командите последователно, по реда на подаването им. Ако първата команда завърши успешно, програмата добавя нейното име към съдържанието на файла, подаден като команден параметър. 
// Ако командата завърши неуспешно, програмата уведомява потребителя чрез подходящо съобщение.

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        errx(1, "Not three arguments");
    }

    char* first_cmd = argv[1];
    char* second_cmd = argv[2];
    int fd = open(argv[3], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
	err(2, "Cannot open file");
    }

    pid_t pid = fork();
    int status;
    if (pid == -1) 
    {
        err(3, "Fork failed");
    } 

    if (pid == 0) 
    {
        if (execlp(first_cmd, first_cmd, (char *) NULL) == -1) 
	{
            err(4, "Error with exec");
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
        ssize_t bytes_written = write(fd, first_cmd, strlen(first_cmd));
        if (bytes_written == -1)
        {
            	err(5, "Error while writing");
        }
    }

    pid = fork();
    if (pid == -1) 
    {
        err(3, "Fork failed");
    } 

    if (pid == 0) 
    {
        if (execlp(second_cmd, second_cmd, (char *) NULL) == -1) 
	{
            err(4, "Error with exec");
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
	ssize_t bytes_written = write(fd, second_cmd, strlen(second_cmd));
        if (bytes_written == -1)
        {
            	err(5, "Error while writing");
        }
    }

    return 0;
}