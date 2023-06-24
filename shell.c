#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
/**
 * main - entry point
 * @argCount: the number of command-line arguments
 * @argArray: array of strings containing the command-line arguments
 * return: 0 success
 */
int main(int argCount ,char **argArray)
{
       	char *promptLine = "simpleshell $:";
	int i = 0;
	char *argPtr ;
	size_t argsize = 0;
	ssize_t getlineReturn = 0;
	char *argPtr_cpy ;
	const char *delimiter = " ";
	int n_tkn = 0;
	char *str_tkn;
	
	while (getlineReturn != -1)
	{
		for (i = 0; promptLine[i] != '\0'; i++)
			_putchar(promptLine[i]);
		getlineReturn = getline(&argPtr, &argsize, stdin);
		if (getlineReturn == -1)
		{
			return (-1);
		}
	}
	argPtr_cpy = malloc(sizeof(char) * getlineReturn);
	if (argPtr_cpy == NULL)
	{
		return (-1);
	}
	_strcpy(argPtr_cpy, argPtr);
	str_tkn = strtok(argPtr_cpy, delimiter);
	while (str_tkn != NULL)
	{
		n_tkn++;
		str_tkn = strtok(NULL, delimiter);
	}
	n_tkn++;
	argArray = malloc(sizeof(char *) * n_tkn);
	str_tkn = strtok(argPtr, delimiter);
	while (str_tkn != NULL)
	{
		int i = 0;
		argArray[i] = malloc(sizeof(char) * _strlen(str_tkn));
		_strcpy(argArray[i], str_tkn);
		str_tkn = strtok(NULL, delimiter);
		i++;
	}
	argArray[i] = NULL;
	execute_cmd(argArray);
	for (i = 0; argPtr[i] != '\0' ; i++)
		_putchar(argPtr[i]);
	free(argPtr);
	free(argPtr_cpy);
	return (0);
}
