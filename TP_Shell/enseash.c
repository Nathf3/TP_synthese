#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#define message "Bienvenue dans le Shell ENSEA. \nPour quitter, tapez 'exit'"

int main() {
    write (STDOUT_FILENO,message, strlen(message));
    return 0;
}