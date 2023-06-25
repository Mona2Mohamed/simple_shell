#include "shell.h"
#include <stdio.h>
#include <sys/wait.h>

/**
 * execute_cmd - function handles commands
 * @argv: argument command
 */
int execute_cmd(char **argv)
{
        pid_t pid = fork();

        if (pid == 0)
        {
                int exec_status = execve(argv[0], argv, NULL);
                if (exec_status == -1)
                {
                        perror("Error: failed to execute command");
                        exit(1);
                }
        }
        else
        {
                int status;
                waitpid(pid, &status, 0);
        }
        return 0;
}
