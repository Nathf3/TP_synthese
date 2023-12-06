#include "enseash.h"

#define welcome_message "Welcome in the ENSEA Shell. \nTo exit, type 'exit' or use CTRL + D"
#define MAX_INPUT_LENGTH 100
#define prompt "\nenseash % "

int main() {
    Welcome_message_display();
    while(1) {
        run_micro_shell();
    }
}

void Welcome_message_display(void){ //display of the welcome's message
write (STDOUT_FILENO,welcome_message, strlen(welcome_message));
}

void run_micro_shell(){// init micro shell and run micro shell

        write(STDOUT_FILENO, prompt, strlen(prompt));
        char buffer[MAX_INPUT_LENGTH];
        int length_command = read(STDIN_FILENO, buffer, MAX_INPUT_LENGTH);
        buffer[length_command - 1] = 0;

        if (!strcmp(buffer,"exit") || (length_command==0)) { //Loop exit command
            write(STDOUT_FILENO, "Bye bye ...", strlen("Bye bye ..."));
            exit(EXIT_SUCCESS);
        }

        int status;
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
            wait(&status);

            if (WIFEXITED(status)) {
                char message[100];
                int exit_status = WEXITSTATUS(status);
                sprintf(message,"code exit : %d\n", exit_status);
                write(STDOUT_FILENO,message,strlen(message));
            }
            else if (WIFSIGNALED(status)) {
                char message1[100];
                int signal_status = WTERMSIG(status);
                sprintf(message1,"signal exit : %d\n", signal_status);
                write(STDOUT_FILENO,message1,strlen(message1));
            }
        }
}