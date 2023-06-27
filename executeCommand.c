#include "shell.h"
#include <stdio.h>
#include <sys/wait.h>
#include <sys/stat.h>
/**
 * execute_cmd - function handles commands
 * @argv: argument command
 */
void execute_cmd(char **argv)
{
	char *cmd = NULL;
	char *new_cmd = NULL;
	pid_t pid = fork();

	if (pid == -1)
	{
		perror("Failed to fork");
		return;
	}
	else if (pid == 0)
	{
		if (argv)
		{
			cmd = argv[0];
			new_cmd = get_loc(cmd);
			if (execve(new_cmd, argv, NULL) == -1)
				perror("Error:");
		}
	}
	else
	{
		int status;

		if (waitpid(pid, &status, 0) == -1)
		{
			perror("Error: failed to wait for child process");
			return;
		}
		if (WIFEXITED(status))
			WEXITSTATUS(status);
	}
}
