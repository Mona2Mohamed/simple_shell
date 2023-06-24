#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int _putchar(char c);
ssize_t getline(char **lineptr, size_t *n, FILE *stream);
char *strtok(char *str, const char *delim);
char *_strcpy(char *dest, char *src);
int _strlen(char *s);
void execute_cmd(char **argArray);
#endif
