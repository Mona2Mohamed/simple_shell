#include "shell.h"
/**
 * is_delimiter - checks if a character is a delimiter
 * @c: character to check
 * @delim: string of delimiters
 * Return: 1 if character is a delimiter, 0 otherwise
 */
int is_delimiter(char c, char *delim)
{
	/**
	 * Iterate through the delimiter string an
	 * check if character matches any delimiter
	 */
	while (*delim)
		if (*delim++ == c)
			return (1);
	return (0);
}
/**
 * convert_to_integer - converts a string to an integer
 * @s: string to be converted
 * Return: 0 if no numbers in string, converted number otherwise
 */
int convert_to_integer(char *s)
{
	int i, sign = 1, flag = 0, output;
	unsigned int result = 0;

	/* Iterate through the string and convert characters to integers */
	for (i = 0; s[i] != '\0' && flag != 2; i++)
	{
		if (s[i] == '-')
			sign *= -1;

		if (s[i] >= '0' && s[i] <= '9')
		{
			flag = 1;
			result *= 10;
			result += (s[i] - '0');
		}
		else if (flag == 1)
			flag = 2;
	}

	/* Set output to the converted number with the correct sign */
	if (sign == -1)
		output = -result;
	else
		output = result;

	return (output);
}
/**
 * check_alpha - checks if a character is alphabetical
 * @c: character to check
 * Return: 1 if character is alphabetical, 0 otherwise
 */
int check_alpha(int c)
{
	/* Check if character is in the range of alphabets */
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	else
		return (0);
}
/**
 * check_interactive - checks if the shell is in interactive mode
 * @info: pointer to a struct containing shell information
 *
 * Return: 1 if shell is in interactive mode, 0 otherwise
 */
int check_interactive(info_t *info)
{
	/**
	 * Check if standard input is a terminal
	 * and readfd is less than or equal to 2
	 */
	return (isatty(STDIN_FILENO) && info->readfd <= 2);
}
