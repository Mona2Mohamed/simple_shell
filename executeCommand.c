#include "shell.h"
/**
 * execute_cmd - function handles commands
 * @argv: argument command
 */
void execute_cmd(char **argv)
{
	char *com = NULL, *actual_com = NULL;
	if (argv)
	{
		com= argv[0];
		actual_com = get_loc(com);

		if (execve(actual_com, argv, NULL) == -1)
			perror("Error:");
	}
}
