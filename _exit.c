#include "shell.h"
/**
 **concatenate_strings_n - concatenates two strings up to a certain length
 *@dest: the destination string to be concatenated to
 *@src: the source string to be concatenated
 *@n: the maximum number of characters to be used for the concatenation
 *Return: a pointer to the destination string
 */
char *concatenate_strings_n(char *dest, char *src, int n)
{
	int i, j;
	char *result = dest;

	i = 0;
	j = 0;
	while (dest[i] != '\0')
		i++;
	while (src[j] != '\0' && j < n)
	{
		dest[i] = src[j];
		i++;
		j++;
	}
	if (j < n)
		dest[i] = '\0';
	return (result);
}
/**
 **copy_string_n - copies a string up to a certain length
 *@dest: the destination string to be copied to
 *@src: the source string
 *@n: the maximum number of characters to be copied
 *Return: a pointer to the destination string
 */
char *copy_string_n(char *dest, char *src, int n)
{
	int i, j;
	char *result = dest;

	i = 0;
	while (src[i] != '\0' && i < n - 1)
	{
		dest[i] = src[i];
		i++;
	}
	if (i < n)
	{
		j = i;
		while (j < n)
		{
			dest[j] = '\0';
			j++;
		}
	}
	return (result);
}
/**
 **find_character_in_string - finds a character in a string
 *@str: the string to be searched
 *@c: the character to look for
 *Return: a pointer to the memory area of the first occurrence of c in str
 */
char *find_character_in_string(char *str, char c)
{
	do {
		if (*str == c)
			return (str);
	} while (*str++ != '\0');

	return (NULL);
}
