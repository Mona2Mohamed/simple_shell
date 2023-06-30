#include "shell.h"
/**
 * help_shell - displays a help message
 * @info: a struct containing potential arguments
 * Return: Always 0
 */
int help_shell(info_t *info)
{
	char **arg_array;

	arg_array = info->argv;
	puts("help call works. Function not yet implemented \n");
	if (0)
		puts(*arg_array); /* temp att_unused workaround */
	return (0);
}
/**
 * exit_shell - exits the shell
 * @info: a struct containing potential arguments
 * Return: exits with a given exit status
 * (0) if info.argv[0] != "exit"
 */
int exit_shell(info_t *info)
{
	int exit_status;

	if (info->argv[1]) /* If there is an exit argument */
	{
		exit_status = print_shell_error(info->argv[1]);
		if (exit_status == -1)
		{
			info->status = 2;
			print_env(info, "Illegal number: ");
			puts(info->argv[1]);
			putchar('\n');
			return (1);
		}
		info->err_num = _erratoi(info->argv[1]);
		return (-2);
	}
	info->err_num = -1;
	return (-2);
}
/**
 * change_directory - changes the current directory of the process
 * @info: a struct containing potential arguments
 * Return: Always 0
 */
int change_directory(info_t *info)
{
	char *cwd_buffer, *dir, buffer[1024];
	int chdir_ret;

	cwd_buffer = getcwd(buffer, 1024);
	if (!cwd_buffer)
		_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!info->argv[1])
	{
		dir = (char) getenv(info, "HOME=");
		if (!dir)
			chdir_ret = /* TODO: what should this be? */
				chdir((dir = (char) _getenv(info, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (_strcmp(info->argv[1], "-") == 0)
	{
		if (!_getenv(info, "OLDPWD="))
		{
			_puts(cwd_buffer);
			putchar('\n');
			return (1);
		}
		_puts(_getenv(info, "OLDPWD=")), _putchar('\n');
		chdir_ret = /* TODO: what should this be? */
			chdir((dir = (char) _getenv(info, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(info->argv[1]);
	if (chdir_ret == -1)
	{
		print_error(info, "can't cd to ");
		_eputs(info->argv[1]), _eputchar('\n');
	}
	else
	{
		setenv(info, "OLDPWD", _getenv(info, "PWD="));
		_setenv(info, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

