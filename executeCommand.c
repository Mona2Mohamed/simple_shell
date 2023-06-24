#include "shell.h"
#include <stdio.h>
/**
 * execute_cmd - function handles commands
 * @argArray: argument command
 */
void execute_cmd(char **argArray)
{
	char *commandLine = NULL;
	
	if (argArray)
	{
		commandLine = argArray[0];
		if (execve(commandLine, argArray, NULL) == -1)
			perror("Error:");
        }
}
