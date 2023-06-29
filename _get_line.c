#include "shell.h"

/**
 * read_cmd_chain_buffer - reads input buffer for chained commands
 * @info: parameter struct
 * @buffer: address of buffer
 * @length: address of length variable
 *
 * Return: bytes read
 */
ssize_t read_cmd_chain_buffer(info_t *info, char **buffer, size_t *length)
{
    ssize_t read_bytes = 0;
    size_t input_length = 0;

    if (!*length) {
        /* Free previously allocated buffer */
        free(*buffer);
        *buffer = NULL;
        signal(SIGINT, sigint_handler);
        /* Read input from stdin */
        read_bytes = getline(buffer, &input_length, stdin);
        if (read_bytes > 0) {
            /* Remove trailing newline */
            if ((*buffer)[read_bytes - 1] == '\n') {
                (*buffer)[read_bytes - 1] = '\0';
                read_bytes--;
            }
            /* Remove comments from input */
            remove_comments(*buffer);
            /* Add input to history list */
            add_to_history_list(info, *buffer, info->history_count++);
            /* Check if input contains command chain */
            if (_strchr(*buffer, ';')) {
                *length = read_bytes;
                info->cmd_chain_buffer = buffer;
            }
        }
    }
    return read_bytes;
}

/**
 * read_input - reads input from stdin
 * @info: parameter struct
 *
 * Return: bytes read
 */
ssize_t read_input(info_t *info)
{
    static char *cmd_chain_buffer; /* buffer for command chain */
    static size_t i = 0, j = 0, buffer_length = 0;
    ssize_t read_bytes = 0;
    char **arg_buffer = &(info->args), *p;

    _putchar(BUFFER_FLUSH);
    /* Read input from command chain buffer if available */
    read_bytes = read_cmd_chain_buffer(info, &cmd_chain_buffer, &buffer_length);
    if (read_bytes == -1) { /* EOF */
        return -1;
    }
    if (buffer_length) { /* command chain buffer not empty */
        j = i; /* Start new iterator at current position */
        p = cmd_chain_buffer + i; /* Get pointer to current command */

        check_cmd_chain(info, cmd_chain_buffer, &j, i, buffer_length);
        /* Iterate to semicolon or end of buffer */
        while (j < buffer_length) {
            if (is_cmd_chain(info, cmd_chain_buffer, &j)) {
                break;
            }
            j++;
        }

        i = j + 1; /* Increment past null semicolon */
        if (i >= buffer_length) { /* Reached end of buffer */
            i = buffer_length = 0; /* Reset buffer position and length */
            info->cmd_chain_buffer_type = NORMAL_CMD;
        }

        *arg_buffer = p; /* Pass pointer to current command */
        return _strlen(p); /* Return length of current command */
    }

    *arg_buffer = cmd_chain_buffer; /* Pass buffer from getline() */
    return read_bytes; /* Return length of buffer from getline() */
}

/**
 * read_buffer - reads a buffer
 * @info: parameter struct
 * @buffer: buffer
 * @length: size
 *
 * Return: bytes read
 */
ssize_t read_buffer(info_t *info, char *buffer, size_t *length)
{
    ssize_t read_bytes = 0;

    if (*length) {
        return 0;
    }
    read_bytes = read(info->readfd, buffer, READ_BUFFER_SIZE);
    if (read_bytes >= 0) {
        *length = read_bytes;
    }
    return read_bytes;
}

/**
 * getline_wrapper - reads input from stdin using getline()
 * @info: parameter struct
 * @ptr: address of pointer to buffer, preallocated or NULL
 * @length: size of preallocated ptr buffer if not NULL
 *
 * Return: bytes read
 */
int getline_wrapper(info_t *info, char **ptr, size_t *length)
{
    static char buffer[READ_BUFFER_SIZE];
    static size_t i = 0, buffer_length = 0;
    size_t k;
    ssize_t read_bytes = 0, total_bytes_read = 0;
    char *p = NULL, *new_p = NULL, *c;

    p = *ptr;
    if (p && length) {
        total_bytes_read = *length;
    }
    if (i == buffer_length) {
        i = buffer_length = 0;
    }

    read_bytes = read_buffer(info, buffer, &buffer_length);
    if (read_bytes == -1 || (read_bytes == 0 && buffer_length == 0)) {
        return -1;
    }

    c =_strchr(buffer + i, '\n');
    k = c ? 1 + (unsigned int)(c - buffer) : buffer_length;
    new_p = _realloc(p, total_bytes_read, total_bytes_read ? total_bytes_read + k : k + 1);
    if (!new_p) { /* MALLOC FAILURE! */
        if (p) {
            free(p);
        }
        return -1;
    }

    if (total_bytes_read) {
        _strncat(new_p, buffer + i, k - i);
    } else {
        _strncpy(new_p, buffer + i, k - i + 1);
    }

    total_bytes_read += k - i;
    i = k;
    p = new_p;

    if (length) {
        *length = total_bytes_read;
    }
    *ptr = p;
    return total_bytes_read;
}

/**
 * sigint_handler - blocks SIGINT signal
 * @sig_num: the signal number
 *
 * Return: void
 */
void sigint_handler(__attribute__((unused))int sig_num)
{
    _puts("\n");
    _puts("$ ");
    _putchar(BUFFER_FLUSH);
}
