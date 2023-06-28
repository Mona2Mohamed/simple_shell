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
	int i;
	const char *delim = " \n";

	(void) argc;
	while (1)
	{
		printf("%s", promptLine);
		getlineReturn =  getline(&inputLine, &inputSize, stdin);
		if (getlineReturn == -1)
		{
			printf("out of shell\n");
			return (-1);
		}

		argPtr_cpy = malloc(sizeof(char) * getlineReturn);
		if (argPtr_cpy == NULL)
		{
			perror("memory allocation error");
			return (-1);
		}
		strcpy(argPtr_cpy, inputLine);
		str_tkn = strtok(argPtr_cpy, delim);
		while (str_tkn != NULL)
		{
			n_tkn++;
			str_tkn = strtok(NULL, delim);
		}
		n_tkn++;
		argv = malloc(sizeof(char *) * n_tkn);
		str_tkn = strtok(inputLine, delim);

		for (i = 0; str_tkn != NULL; i++)
		{
			argv[i] = malloc(sizeof(char) * strlen(str_tkn));
			strcpy(argv[i], str_tkn);
			str_tkn = strtok(NULL, delim);
		}
		argv[i] = NULL;
		execute_cmd(argv);
	}

	free(argPtr_cpy);
	free(inputLine);
	return (0);
}
