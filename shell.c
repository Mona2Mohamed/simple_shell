#include <string.h>
#include <sys/wait.h>
#include "shell.h"

/**
 * main - entry point
 * @argc: the number of command-line arguments
 * @argv: array of strings containing the command-line arguments
 * return: 0 success
 */
int main(int argc, char **argv)
{
        char *promptLine = "simpleshell $:";
        char *inputLine = NULL;
        size_t inputSize = 0;
        int n_tkn = 0;
        char *argPtr_cpy = NULL;
        char *str_tkn = NULL;

        (void) argc;
        while (1)
        {
                int i = 0;
		char *path = getenv("PATH");
		int exec_status = 0;

                for (i = 0; promptLine[i] != '\0'; i++)
                        putchar(promptLine[i]);
                if (getline(&inputLine, &inputSize, stdin) == -1)
                {
                        fprintf(stderr, "Error: failed to read input\n");
                        return 1;
                }
                argPtr_cpy = strdup(inputLine);
                if (!argPtr_cpy)
                {
                        fprintf(stderr, "Error: failed to allocate memory\n");
                        return 1;
                }
                str_tkn = strtok(argPtr_cpy, " \t\n");
                while (str_tkn)
                {
                        n_tkn++;
                        str_tkn = strtok(NULL, " \t\n");
                }
                argv = malloc((n_tkn + 1) * sizeof(char *));
                if (!argv)
                {
                        fprintf(stderr, "Error: memory allocation failed\n");
                        free(argPtr_cpy);
                        return 1;
                }
                str_tkn = strtok(inputLine, " \t\n");
                while (str_tkn)
                {
                        argv[i] = strdup(str_tkn);
                        if (!argv[i])
                        {
                                int j = 0;

                                fprintf(stderr, "Error: memory allocation failed\n");
                                for (j = 0; j < i; j++)
                                        free(argv[j]);
                                free(argv);
                                free(argPtr_cpy);
                                return 1;
                        }
                        str_tkn = strtok(NULL, " \t\n");
                        i++;
                }
                argv[n_tkn] = NULL;

                if (path == NULL)
                {
                        fprintf(stderr, "Error: $PATH environment variable is not defined\n");
                        exit(1);
                }
                exec_status = execute_cmd(argv);

                if (exec_status != 0)
                {
                        fprintf(stderr, "Error: command execution failed\n");
                }

                for (i = 0; i < n_tkn; i++)
                        free(argv[i]);
                free(argv);
                free(argPtr_cpy);
        }
        free(inputLine);
        return 0;
}
