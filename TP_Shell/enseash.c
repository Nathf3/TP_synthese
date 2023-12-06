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

void Welcome_message_display(void){ //Display the welcome message
    write (STDOUT_FILENO,welcome_message, strlen(welcome_message));
    write(STDOUT_FILENO, prompt, strlen(prompt));
}

void run_micro_shell(){// Initialize and run micro shell

        char buffer[MAX_INPUT_LENGTH];
        int length_command = read(STDIN_FILENO, buffer, MAX_INPUT_LENGTH);
        buffer[length_command - 1] = 0;

        if (!strcmp(buffer,"exit") || (length_command==0)) { //Exit commands
            write(STDOUT_FILENO, "Bye bye ...", strlen("Bye bye ..."));
            exit(EXIT_SUCCESS);
        }

        double time_spent = 0.0;
        clock_t begin = clock();   //Start the execution time measurement

        int status;
        pid_t childPid = fork();
        if (childPid == -1) {   //Error creating the child process
            write(STDERR_FILENO, "Error during child processor creation",strlen("Error during child processor creation"));
        }
        else if (childPid == 0) {   //Child process
            execlp(buffer, buffer, (char *) NULL);
            write(STDERR_FILENO, "Error during command execution\n", strlen("Error during command execution\n"));
            exit(EXIT_FAILURE);
        }
        else {   //Father process
            wait(&status);
            clock_t end = clock();   //End the execution time measurement
            time_spent += 1000 * (double)(end - begin) / CLOCKS_PER_SEC;

            display_signal_or_exit_code(status,time_spent);
        }
}

void display_signal_or_exit_code(int status,double time_spent){
    char message_exit[100];
    if (WIFEXITED(status)) {
        int exit_status = WEXITSTATUS(status);
        sprintf(message_exit,"\nenseash [exit : %d | %.3f ms] %% ", exit_status,time_spent);
        write(STDOUT_FILENO,message_exit,strlen(message_exit));
    }
    else if (WIFSIGNALED(status)) {
        int signal_status = WTERMSIG(status);
        sprintf(message_exit,"\nenseash [sign : %d | %.3f ms] %% ", signal_status,time_spent);
        write(STDOUT_FILENO,message_exit,strlen(message_exit));
    }
}