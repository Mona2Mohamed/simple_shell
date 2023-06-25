#include "shell.h"

/**
*_strlen-determine string length
*@s:String pointer
*Return: returns string length
*/
int _strlen(char *s)
{
	int p = 0;

	while (*(s + p) != 0)
	{
		p++;
	}
	return (p);
}
