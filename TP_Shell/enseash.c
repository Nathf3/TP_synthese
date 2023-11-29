
#include "enseash.h"

#define welcome_message "Welcom in the ENSEA's Shell. \nTo exit, type 'exit'\n"
#define MAX_INPUT_LENGTH 100
#define prompt "enseash %"

int main() {
    write (STDOUT_FILENO,welcome_message, strlen(welcome_message));
    while(1) {
        write(STDOUT_FILENO, prompt, strlen(prompt));
        char buffer[MAX_INPUT_LENGTH];
        int length_command = read(STDIN_FILENO, buffer, MAX_INPUT_LENGTH);
        buffer[length_command - 1] = 0;

        pid_t childPid = fork();
        if (childPid == -1) {
            write(STDERR_FILENO, "Error during child processor creation",
                  strlen("Error during child processor creation"));
        }
        else if (childPid == 0) {
            execlp(buffer, buffer, (char *) NULL);
            write(STDERR_FILENO, "Error during command execution\n", strlen("Error during command execution\n"));
            exit(EXIT_FAILURE);
        }
        else {
            waitpid(childPid, NULL, 0);
        }
    }

}