#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for convert_number() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;

/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 *struct passinfo - contains pseudo-arguements to pass into a function,
 *		allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@histcount: the history line number count
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}
/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;

/* toem_loop.c*/
int run_shell_loop(info_t *shell_info, char **argv);
int find_builtin_command(info_t *shell_info);
void find_command_in_path(info_t *shell_info);
void fork_command(info_t *shell_info);

/* toem_parser.c*/
int is_command(info_t *info, char *path);
char *dup_chars(char *pathstr, int start, int stop);
char *find_path(info_t *info, char *pathstr, char *cmd);

/* loophsh.c */
int loophsh(char **);

/*toem_string1.c*/
char *copy_string(char *destination, char *source);
char *duplicate_string(const char *str);
void print_string(char *str);
int write_character(char c);

/* string.c*/
int _strlen(char *s);
int _strcmp(char *s1, char *s2);
char *starts_with(const char *haystack, const char *needle);
char *_strcat(char *dest, char *src);

/*errors*/
void print_error_string(char *str);
int _putfd(char c, int fd);
int _putsfd(char *str, int fd);
int _put_error_char(char c);

/*errorsv1*/
void print_shell_error(info_t *info, char *error_type);
void remove_comments_from_string(char *str);
char *convert_number_to_string(long int num, int base, int flags);
int convert_string_to_int(char *str);
int print_integer(int input, int fd);

/*exit*/
char *concatenate_strings_n(char *dest, char *src, int n);
char *copy_string_n(char *dest, char *src, int n);
char *find_character_in_string(char *str, char c);

/*token*/
char **split_string2(char *input_string, char delimiter);
char **split_string(char *input_string, char *delimiter_string);

/*realloc*/
char *_memset(char *s, char b, unsigned int n);
void ffreee(char **pp);
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);
/*memo*/
int bfreee(void **ptr);

/*_stoi.c*/
int is_delimiter(char c, char *delim);
int convert_to_integer(char *s);
int check_alpha(int c);
int check_interactive(info_t *info);

/*builtin*/
int help_shell(info_t *info);
int exit_shell(info_t *info);
int change_directory(info_t *info);

/*builtinv1*/
int set_alias(info_t *info, char *str);
int alias_shell(info_t *info);
int print_alias(list_t *node);
int unset_alias(info_t *info, char *str);
int print_history(info_t *info);

/*getline*/
ssize_t buffer_input(info_t *info, char **buf, size_t *len);
ssize_t get_input(info_t *info);
ssize_t read_buffer(info_t *info, char *buf, size_t *i);
int _getline(info_t *info, char **ptr, size_t *length);
void block_ctrl_c(__attribute__((unused))int sig_num);

/* get info*/
void clear_info_struct(info_t *info);
void set_info_struct(info_t *info, char **av);
void free_info_struct(info_t *info, int all);

/* environ*/
char *get_env_var(info_t *info, const char *name);
int populate_env_list(info_t *info);
int unset_env_var(info_t *info);
int set_env_var(info_t *info);
int print_env(info_t *info);

/*getenv.c*/
char **get_environment(info_t *info);
int unset_environment_variable(info_t *info, char *variable);
int set_environment_variable(info_t *info, char *variable, char *value);

/* hsitory.c*/
char *get_history_filename(info_t *info);
int write_history(info_t *info);
int read_history(info_t *info);
int build_history_list(info_t *info, char *buf, int linecount);
int renumber_history(info_t *info);

/*listv0.c*/
list_t *prepend_node(list_t **head_ptr, const char *str_value, int num_value);
list_t *append_node(list_t **head_ptr, const char *str_value, int num_value);
size_t print_string_list(const list_t *head);
int remove_node_at_index(list_t **head_ptr, unsigned int index);
void free_list_t(list_t **head_ptr);

/* listv1.c*/
ssize_t get_node_index(list_t *head, list_t *node);
list_t *find_node_starts_with(list_t *node, char *prefix, char c);
size_t print_my_list(const list_t *head);
char **list_to_str_array(list_t *head);
size_t get_list_len(const list_t *head);

/*varse.c*/
int replace_string(char **old, char *new);
int replace_vars(info_t *info);
int replace_alias(info_t *info);
void check_chain(info_t *info, char *buf, size_t *p, size_t i, size_t len);
int is_chain(info_t *info, char *buf, size_t *p);

#endif


