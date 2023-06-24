// Зад. 81 2017-IN-02 Напишете програма на C, която приема незадължителен параметър – име на команда. Ако не е зададена команда като параметър, да се ползва командата echo. 
// Максималната допустима дължина на командата е 4 знака. Програмата чете низове (с максимална дължина 4 знака) от стандартния си вход, разделени с интервали (0x20) или знак за нов ред (0x0A). 
// Ако някой низ е с дължина по-голяма от 4 знака, то програмата да терминира със съобщение за грешка. Подадените на стандартния вход низове програмата трябва да третира като множество от параметри 
// за дефинираната команда. Програмата ви трябва да изпълни командата колкото пъти е необходимо с максимум два низа като параметри, като изчаква изпълнението да приключи, преди да започне ново изпълнение.
// Примерни вход, извиквания и изходи:
// $ cat f1
// a1
// $ cat f2
// a2
// $ cat f3
// a3
// $ echo -e "f1\nf2 f3" | ./main cat
// a1
// a2
// a3
// $ echo -e "f1\nf2 f3" | ./main
// f1 f2
// f3

#include <err.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

void exec_command(char arguments[][5], char* command)
{
    int pid = fork();
    if (pid == -1)
    {
        err(2, "Cannot fork");
    }
    if (pid == 0)
    {
        char* execv_arguments[5];
        execv_arguments[0] = command;
        int i = 1;
        int j = 0;

        while (arguments[j][0] != '\0')
        {
            execv_arguments[i] = arguments[j];
            i++;
            j++;
        }
        execv_arguments[i] = NULL;
        if (execvp(command, execv_arguments) == -1)
        {
            err(3, "Couldn't exec.");
        }
    }

    wait(NULL);
}

int main(const int argc, const char* argv[])
{
    char command[5];
    if (argc == 1)
    {
        strcpy(command, "echo");
    }
    else if (argc == 2 && strlen(argv[1]) < 5)
    {
        strcpy(command, argv[1]);
    }
    else
    {
        errx(1, "Wrong number of parameters.");
    }

    char arguments[5];
    int index = 0;
    int count = 0;
    char command_arguments[3][5];
    ssize_t read_bytes;
    while (((read_bytes = read(0, arguments + index, 1)) > 0) && index < 5)
    {
        if (count == 2)
        {
            command_arguments[2][0] = '\0';
            exec_command(command_arguments, command);
            count = 0;
        }
        if (arguments[index] == '\n' || arguments[index] == ' ')
        {
            arguments[index] = '\0';
            strcpy(command_arguments[count], arguments);
            index = 0;
            count++;
        }
        else
        {
            ++index;
        }
    }

    if (read_bytes == -1)
    {
        err(4, "Error reading");
    }

    if (count > 0)
    {
        command_arguments[count][0] = '\0';
        exec_command(command_arguments, command);
    }

    exit(0);
}