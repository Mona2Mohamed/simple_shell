#include "shell.h"

/**
 * get_environment - Returns a copy of the environment variables as an array
 * @info: Struct containing potential arguments.
 *
 * Return: A copy of the environment variables as an array.
 */
char **get_environment(info_t *info)
{
	if (!info->env_array || info->env_changed)
	{
		info->env_array = list_to_array(info->env);
		info->env_changed = 0;
	}

	return (info->env_array);
}

/**
 * unset_environment_variable - Remove an environment variable
 * @info: Struct containing potential arguments.
 * @variable: The name of the environment variable to remove.
 *
 * Return: 1 on successful removal, 0 otherwise.
 */
int unset_environment_variable(info_t *info, char *variable)
{
	list_t *node = info->env;
	size_t i = 0;
	char *p;

	if (!node || !variable)
		return (0);

	while (node)
	{
		p = starts_with(node->str, variable);
		if (p && *p == '=')
		{
			info->env_changed = delete_node_at_index(&(info->env), i);
			i = 0;
			node = info->env;
			continue;
		}
		node = node->next;
		i++;
	}
	return (info->env_changed);
}

/**
 * set_environment_variable - Initialize a new environment variable or
 *                             modify an existing one
 * @info: Struct containing potential arguments.
 * @variable: The name of the environment variable to initialize or modify.
 * @value: The value to set for the environment variable.
 *
 * Return: Always 0.
 */
int set_environment_variable(info_t *info, char *variable, char *value)
{
	char *buffer = NULL;
	list_t *node;
	char *p;

	if (!variable || !value)
		return (0);

	buffer = malloc(_strlen(variable) + _strlen(value) + 2);
	if (!buffer)
		return (1);

	_strcpy(buffer, variable);
	_strcat(buffer, "=");
	_strcat(buffer, value);

	node = info->env;
	while (node)
	{
		p = starts_with(node->str, variable);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buffer;
			info->env_changed = 1;
			return (0);
		}
		node = node->next;
	}

	add_node_end(&(info->env), buffer, 0);
	free(buffer);
	info->env_changed = 1;
	return (0);
}
