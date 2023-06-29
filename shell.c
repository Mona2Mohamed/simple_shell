#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024
/*
 * main - main
 * retrun : 0
 */
int main() {
    char* command = NULL;
    size_t command_size = 0;
    pid_t pid;

    while (1) {
        printf("simple_shell>$ ");
        if (getline(&command, &command_size, stdin) == -1) {
            break;
        }
        command[strcspn(command, "\n")] = '\0';
        if (strcmp(command, "exit") == 0) {
            break;
        }
        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
	    } else if (pid == 0) {
            execlp(command, command, NULL);
            printf("%s: command not found\n", command);
            exit(EXIT_FAILURE);
        } else {
            /*Parent process*/
            waitpid(pid, NULL, 0);
        }
    }
    free(command);
    printf("\n");
    return 0;
}
