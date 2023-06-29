#include "shell.h"
/**
 * get_env_var - gets the value of an environment variable
 * @info: a struct containing potential arguments
 * @name: the name of the environment variable
 *
 * Return: the value of the environment variable
 */
char *get_env_var(info_t *info, const char *name)
{
	list_t *node = info->env;
	char *p;

	while (node)
	{
		p = starts_with(node->str, name);
		if (p && *p)
			return (p);
		node = node->next;
	}
	return (NULL);
}
/**
 * populate_env_list - populates the env linked list
 * @info: a struct containing potential arguments
 * Return: Always 0
 */
int populate_env_list(info_t *info)
{
	list_t *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		add_node_end(&node, environ[i], 0);
	info->env = node;
	return (0);
}
/**
 * unset_env_var - unsets an environment variable
 * @info: a struct containing potential arguments
 * Return: Always 0
 */
int unset_env_var(info_t *info)
{
	int i;

	if (info->argc == 1)
	{
		_eputs("Too few arguments.\n");
		return (1);
	}
	for (i = 1; i <= info->argc; i++)
		_unsetenv(info, info->argv[i]);

	return (0);
}
/**
 * set_env_var - sets an environment variable
 * @info: a struct containing potential arguments
 * Return: Always 0
 */
int set_env_var(info_t *info)
{
	if (info->argc != 3)
	{
		_eputs("Incorrect number of arguments\n");
		return (1);
	}
	if (_setenv(info, info->argv[1], info->argv[2]))
		return (0);
	return (1);
}
/**
 * print_env - prints the current environment
 * @info: a struct containing potential arguments
 * Return: Always 0
 */
int print_env(info_t *info)
{
	print_list_str(info->env);
	return (0);
}
