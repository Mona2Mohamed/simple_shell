#include "shell.h"

/**
 * get_history_filename - Gets the history file name
 * @info: Pointer to the info_t struct
 *
 * Return: Allocated string containing history file name
 */
char *get_history_filename(info_t *info)
{
	char *home, *filename;

	home = _getenv(info, "HOME=");
	if (!home)
		return (NULL);

	filename = malloc(sizeof(char) * (_strlen(home) + _strlen(HIST) + 2));
	if (!filename)
		return (NULL);

	filename[0] = '\0';
	_strcpy(filename, home);
	_strcat(filename, "/");
	_strcat(filename, HIST);

	free(home);
	return (filename);
}

/**
 * write_history - Writes history to a file
 * @info: Pointer to the info_t struct
 *
 * Return: 1 on success, -1 on failure
 */
int write_history(info_t *info)
{
	char *filename;
	ssize_t fd;
	list_t *node;

	filename = get_history_filename(info);
	if (!filename)
		return (-1);
	fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (node = info->history; node; node = node->next)
	{
		_putsfd(node->str, fd);
		_putfd('\n', fd);
	}
	_putfd(BUF_FLUSH, fd);
	close(fd);
	return (1);
}

/**
 * read_history - Reads history from a file
 * @info: Pointer to the info_t struct
 *
 * Return: The number of lines read from history file
 */
int read_history(info_t *info)
{
	char *filename, *buf;
	struct stat st;
	ssize_t fd, rdlen, fsize;
	int i, last = 0, linecount = 0;

	filename = get_history_filename(info);
	if (!filename)
		return (0);
	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (fstat(fd, &st))
		return (close(fd), 0);
	fsize = st.st_size;
	if (fsize < 2)
		return (close(fd), 0);
	buf = malloc(sizeof(char) * (fsize + 1));
	if (!buf)
		return (close(fd), 0);
	rdlen = read(fd, buf, fsize);
	buf[fsize] = '\0';
	if (rdlen <= 0)
		return (free(buf), close(fd), 0);
	close(fd);
	for (i = 0; i < fsize; ++i)
	{
		if (buf[i] == '\n')
		{
			buf[i] = '\0';
			build_history_list(info, buf + last, linecount++);
			last = i + 1;
		}
	}
	if (last != i)
		build_history_list(info, buf + last, linecount++);
	free(buf);
	info->histcount = linecount;
	while (info->histcount-- >= HIST)
		delete_node_at_index(&(info->history), 0);
	renumber_history(info);
	return (info->histcount);
}
/**
 * build_history_list - Adds an entry to the history linked list
 * @info: Pointer to the info_t struct
 * @buf: The buffer containing the history entry to add
 * @linecount: The line count of the history entry
 * Return: Always 0
 */
int build_history_list(info_t *info, char *buf, int linecount)
{
	list_t *node;

	node = NULL;
	if (info->history)
		node = info->history;
	add_node_end(&node, buf, linecount);

	if (!info->history)
		info->history = node;

	return (0);
}

/**
 * renumber_history - Renumber the history linked list after changes
 * @info: Pointer to the info_t struct
 *
 * Return: The new histcount
 */
int renumber_history(info_t *info)
{
	list_t *node;
	int i;

	node = info->history;
	i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}

	info->histcount = i;
	return (info->histcount);
}
