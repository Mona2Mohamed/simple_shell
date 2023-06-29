#include "shell.h"
/**
 * print_shell_error - prints an error message to standard error
 * @info: a struct containing potential arguments and return values
 * @error_type: string containing specified error type
 * Return: void
 */
void print_shell_error(info_t *info, char *error_type)
{
	_eputs(info->fname);
	_eputs(": ");
	print_integer(info->line_count, STDERR_FILENO);
	_eputs(": ");
	_eputs(info->argv[0]);
	_eputs(": ");
	_eputs(error_type);
}
/**
 * remove_comments_from_string - replaces first instance of '#' with '\0'
 * @str: the string to modify
 * Return: void
 */
void remove_comments_from_string(char *str)
{
	int i;

	for (i = 0; str[i] != '\0'; i++)
		if (str[i] == '#' && (!i || str[i - 1] == ' '))
		{
			str[i] = '\0';
			break;
		}
}
/**
 * convert_number_to_string - converts a number to a string
 * @num: the number to be converted
 * @base: the base to be used for the conversion
 * @flags: flags to modify the output format
 * Return: the converted string
 */
char *convert_number_to_string(long int num, int base, int flags)
{
	static char *array;
	static char buffer[50];
	char sign = 0;
	char *ptr;
	unsigned long n = num;

	if (!(flags & CONVERT_UNSIGNED) && num < 0)
	{
		n = -num;
		sign = '-';

	}
	array = flags & CONVERT_LOWERCASE ? "0123456789abcdef" : "0123456789ABCDEF";
	ptr = &buffer[49];
	*ptr = '\0';

	do	{
		*--ptr = array[n % base];
		n /= base;
	} while (n != 0);

	if (sign)
		*--ptr = sign;
	return (ptr);
}
/**
 * convert_string_to_int - converts a string to an integer
 * @str: the string to be converted
 * Return: 0 if no numbers in string, converted number otherwise
 *       -1 on error
 */
int convert_string_to_int(char *str)
{
	int i = 0;
	unsigned long int result = 0;

	if (*str == '+')
		str++;
	for (i = 0;  str[i] != '\0'; i++)
	{
		if (str[i] >= '0' && str[i] <= '9')
		{
			result *= 10;
			result += (str[i] - '0');
			if (result > INT_MAX)
				return (-1);
		}
		else
			return (-1);
	}
	return (result);
}
/**
 * print_integer - prints an integer to a file descriptor
 * @input: the integer to be printed
 * @fd: the file descriptor to write to
 * Return: the number of characters printed
 */
int print_integer(int input, int fd)
{
	int (*__putchar)(char) = _putchar;
	int i, count = 0;
	unsigned int absolute_value, current;

	if (fd == STDERR_FILENO)
		__putchar = _eputchar;
	if (input < 0)
	{
		absolute_value = -input;
		__putchar('-');
		count++;
	}
	else
		absolute_value = input;
	current = absolute_value;
	for (i = 1000000000; i > 1; i /= 10)
	{
		if (absolute_value / i)
		{
			__putchar('0' + current / i);
			count++;
		}
		current %= i;
	}
	__putchar('0' + current);
	count++;

	return (count);
}
