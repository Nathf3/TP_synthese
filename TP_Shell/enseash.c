#include "enseash.h"

#define welcome_message "Welcome in the ENSEA Shell. \nTo exit, type 'exit'"
#define MAX_INPUT_LENGTH 100
#define prompt "\nenseash % "

int main() {
    write (STDOUT_FILENO,welcome_message, strlen(welcome_message));
    while(1) {
        write(STDOUT_FILENO, prompt, strlen(prompt));
        char buffer[MAX_INPUT_LENGTH];
        int length_command = read(STDIN_FILENO, buffer, MAX_INPUT_LENGTH);
        buffer[length_command - 1] = 0;

        if (!strcmp(buffer,"exit") || (length_command==0)) { //Loop exit command
            write(STDOUT_FILENO, "Bye bye ...", strlen("Bye bye ..."));
            exit(EXIT_SUCCESS);
        }

        pid_t childPid = fork();
        if (childPid == -1) { //Error creating the child process
            write(STDERR_FILENO, "Error during child processor creation",
                  strlen("Error during child processor creation"));
        }
        else if (childPid == 0) { //Child process
            execlp(buffer, buffer, (char *) NULL);
            write(STDERR_FILENO, "Error during command execution\n", strlen("Error during command execution\n"));
            exit(EXIT_FAILURE);
        }
        else { //Father process
            waitpid(childPid, NULL, 0);
        }
    }
}