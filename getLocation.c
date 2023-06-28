#include "shell.h"
/**
 * get_loc - Get the length of the command given
 * @cmd: command
 * Return: null
 */
char *get_loc(char *cmd)
{
	char *path, *path_cpy, *path_tkn, *file_path;
	int cmd_len;
	int dir_len;
	struct stat buffer;

	path = getenv("PATH");

	if (path)
	{
		path_cpy = strdup(path);
		cmd_len = strlen(cmd);
		path_tkn = strtok(path_cpy, ":");

		while (path_tkn != NULL)
		{
			dir_len = strlen(path_tkn);
			file_path = malloc(cmd_len + dir_len + 2);
			strcpy(file_path, path_tkn);
			strcat(file_path, "/");
			strcat(file_path, cmd);
			strcat(file_path, "\0");
			if (stat(file_path, &buffer) == 0)
			{
				free(path_cpy);
				return (file_path);
			}
			else
			{
				free(file_path);
				path_tkn = strtok(NULL, ":");
			}
		}
		free(path_cpy);
		if (stat(cmd, &buffer) == 0)
			return (cmd);
		return (NULL);
	}
	return (NULL);
}
