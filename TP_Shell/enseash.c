
#define message "Bienvenue dans le Shell ENSEA. \nPour quitter, tapez 'exit'"
#include "enseash.h"

int main() {
    Q1();
    return 0;
}

void Q1() {
    write (STDOUT_FILENO,message, strlen(message));
}