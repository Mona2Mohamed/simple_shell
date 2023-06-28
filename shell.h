#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
int _putchar(char c);
char *get_loc(char *cmd);
char *strtok(char *str, const char *delim);
char *_strcpy(char *dest, char *src);
int _strlen(char *s);
void execute_cmd(char **argv);
#endif
