#include "shell.h"
/**
 * main - entry point
 * @argc: the number of command-line arguments
 * @argv: array of strings containing the command-line arguments
 * return: 0 success
 */
int main(int argc, char **argv)
{
	char *promptLine = "simpleshell:";
	char *inputLine = NULL;
	size_t inputSize = 0;
	int n_tkn = 0;
	char *argPtr_cpy = NULL;
	char *str_tkn = NULL;
	ssize_t getlineReturn = 0;

	(void) argc;
	while (1)
	{
		int i = 0;
		char *path = getenv("PATH");
		const char *delim = " ";

		for (i = 0; promptLine[i] != '\0'; i++)
			_putchar(promptLine[i]);
		getlineReturn =  getline(&inputLine, &inputSize, stdin);
		if (getlineReturn == -1)
		{
			_putchar('\n');
			return (-1);
		}
		/*
		 * printf("%s\n",inputLine);test line
		 * we can divide here by giving inputline and getlinereturn as argument
		 */
		argPtr_cpy = malloc(sizeof(char) * getlineReturn);
		_strcpy(argPtr_cpy, inputLine);
		if (argPtr_cpy == NULL)
		{
			perror("memory allocation error");
			return (-1);
		}
		str_tkn = strtok(argPtr_cpy, delim);
		while (str_tkn != NULL)
		{
			n_tkn++;
			str_tkn = strtok(NULL, delim);
		}
		argv = malloc((n_tkn + 1) * sizeof(char *));
		str_tkn = strtok(inputLine, delim);
		if (argv == NULL)
		{
			perror("argv memory allocation failed");
			free(argPtr_cpy);
			return (1);
		}
		for (i = 0; str_tkn != NULL; i++)
		{
			argv[i] = malloc(sizeof(char) * _strlen(str_tkn));
			_strcpy(argv[i], str_tkn);
			str_tkn = strtok(NULL, delim);
		}
		argv[i] = NULL;
		if (path == NULL)
		{
			perror("Error: $PATH environment variable is not defined\n");
			exit(1);
		}
		execute_cmd(argv);
		for (i = 0; i < n_tkn; i++)
			free(argv[i]);
		free(argv);
		free(argPtr_cpy);
	}
	free(inputLine);
	return (0);
}
