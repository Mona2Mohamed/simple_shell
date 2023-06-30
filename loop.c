#include "shell.h"

/**
 * run_shell_loop - main shell loop
 * @shell_info: pointer to shell information struct
 * @argv: argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int run_shell_loop(info_t *shell_info, char **argv)
{
	ssize_t read_status = 0;
	int builtin_ret_val = 0;

	while (read_status != -1 && builtin_ret_val != -2)
	{
		clear_info(shell_info);
		if (interactive(shell_info))
			_puts("$ ");
		_eputchar(BUF_FLUSH);
		read_status = get_input(shell_info);
		if (read_status != -1)
		{
			set_info(shell_info, argv);
			builtin_ret_val = find_builtin_command(shell_info);
			if (builtin_ret_val == -1)
				find_command_in_path(shell_info);
		}
		else if (interactive(shell_info))
			_putchar('\n');
		free_info(shell_info, 0);
	}
	write_history(shell_info);
	free_info(shell_info, 1);
	if (!interactive(shell_info) && shell_info->status)
		exit(shell_info->status);
	if (builtin_ret_val == -2)
	{
		if (shell_info->error_num == -1)
			exit(shell_info->status);
		exit(shell_info->error_num);
	}
	return (builtin_ret_val);
}

/**
 * find_builtin_command - finds a builtin command
 * @shell_info: pointer to shell information struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int find_builtin_command(info_t *shell_info)
{
	int i, builtin_ret_val = -1;
	builtin_table builtin_table[] = {
		{"exit", _myexit},
		{"env", _myenv},
		{"help", _myhelp},
		{"history", _myhistory},
		{"setenv", _mysetenv},
		{"unsetenv", _myunsetenv},
		{"cd", _mycd},
		{"alias", _myalias},
		{NULL, NULL}
	};

	for (i = 0; builtin_table[i].command; i++)
		if (_strcmp(shell_info->argv[0], builtin_table[i].command) == 0)
		{
			shell_info->line_count++;
			builtin_ret_val = builtin_table[i].func(shell_info);
			break;
		}
	return (builtin_ret_val);
}

/**
 * find_command_in_path - finds a command in PATH
 * @shell_info: pointer to shell information struct
 *
 * Return: void
 */
void find_command_in_path(info_t *shell_info)
{
	char *path = NULL;
	int i, arg_count;

	shell_info->path = shell_info->argv[0];
	if (shell_info->line_count_flag == 1)
	{
		shell_info->line_count++;
		shell_info->line_count_flag = 0;
	}
	for (i = 0, arg_count = 0; shell_info->arg[i]; i++)
		if (!is_delimiter(shell_info->arg[i], " \t\n"))
			arg_count++;
	if (!arg_count)
		return;

	path = find_path(shell_info, _getenv(shell_info, "PATH="), shell_info->argv[0]);
	if (path)
	{
		shell_info->path = path;
		fork_command(shell_info);
	}
	else
	{
		if ((is_interactive(shell_info) || _getenv(shell_info, "PATH=")
			|| shell_info->argv[0][0] == '/') && is_command(shell_info, shell_info->argv[0]))
			fork_command(shell_info);
		else if (*(shell_info->arg) != '\n')
		{
			shell_info->status = 127;
			print_error(shell_info, "not found\n");
		}
	}
}

/**
 * fork_command - forks a an exec thread to run cmd
 * @shell_info: pointer to shell information struct
 *
 * Return: void
 */
void fork_command(info_t *shell_info)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(shell_info->path, shell_info->argv, get_environment(shell_info)) == -1)
		{
			free_shell_info(shell_info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(shell_info->status));
		if (WIFEXITED(shell_info->status))
		{
			shell_info->status = WEXITSTATUS(shell_info->status);
			if (shell_info->status == 126)
				print_error(shell_info, "Permission denied\n");
		}
	}
}
