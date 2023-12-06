# TP de Synthèse – Ensea in the Shell
L'objectif de ce TP est de réaliser un micro shell enseash qui doit servir à lancer des commandes
et à afficher des informations sur leurs exécutions.

## Question 1
> On souhaite ecrire un message de bienvenue pour démarer le programme.  

Pour ce faire on utilise la commande :  
```ruby
#define welcome_message "Welcome in the ENSEA Shell. \nTo exit, type 'exit'\n"
#define MAX_INPUT_LENGTH 100
write (STDOUT_FILENO,welcome_message, strlen(welcome_message));
```
Avec le descripteur de fichier def en sortie (0) le message welcome_message et sa taille en octet
On obtient donc :
```
Welcome in the ENSEA Shell. 
To exit, type 'exit'
enseash %
```
## Question 2
> On souhaite maintenant récupérer la commande de notre terminal ,l'executer puis revenir au prompteur de base `enseash %`

On commence par mettre tous mettre dans une boucle infini pour que notre terminal fonctionne à l'infini:  
`#define prompt "\nenseash % "`
```ruby
while(1) {
write(STDOUT_FILENO, prompt, strlen(prompt));//display of the prompter
char buffer[MAX_INPUT_LENGTH];
int length_command = read(STDIN_FILENO, buffer, MAX_INPUT_LENGTH);//save the command from he terminal in a buffer
buffer[length_command - 1] = 0; //make a limit of the command
...
}
```
On affiche le prompter et on récupère la commande du terminal pour le mettre dans un buffer 
On fait ensuite un fork pour que l'on puisse réiterer le processus :

```ruby
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
  ```
Apres que le fils sera executé donc que la dernière commande sera entrée et validée,le fils sera delete pour laisser place au père qui refait donc une 
demande de de commande .

## Question 3 
>on souhaite maintenant pouvoir quitte le terminal soit en tappant exit soit en faisant ctrl+D

Pour ce faire on détecte quand la commande exit et entrée puis on sort du programme 
```ruby
if (!strcmp(buffer,"exit") || (length_command==0)) { //Loop exit command
write(STDOUT_FILENO, "Bye bye ...", strlen("Bye bye ..."));
exit(EXIT_SUCCESS);
}
```
Pour détecter le ctrl+D il faut regarder si apres avoir tapé la commande le buffer c'est vidé 
on regarde donc si la taille de la commande reçus est null : `(length_command==0)` .

## Question 4
>on souhaite maintenant affichage du code de retour (ou du signal) de la commande précédente dans le prompt

Pour ce faire on fait appel aux fonctions `WIFEXITED(status)` et 'WIFSIGNALED(status)' qui servent à récuperer le code d'exit 
et le code du signal de sortie :
```ruby
char message_exit[100];
if (WIFEXITED(status)) {
    int exit_status = WEXITSTATUS(status);
    sprintf(message_exit,"\nenseahsh [exit:%d] %%", exit_status);
    write(STDOUT_FILENO,message_exit,strlen(message_exit));
}
else if (WIFSIGNALED(status)) {
    int signal_status = WTERMSIG(status);
    sprintf(message_exit,"\nenseash [sign:%d] %%", signal_status);
    write(STDOUT_FILENO,message_exit,strlen(message_exit));
}
``` 
On obtient donc le résulats suivant :
```ruby 
enseash % wc

enseash [sign:9] %ls
build.ninja  CMakeCache.txt  CMakeFiles  cmake_install.cmake  Testing  TP_Shell

enseahsh [exit:0] %aaaa

Error during command execution
enseahsh [exit:1] %
```
Pour obtenir le sig 9 on à du lancer un processus comme `>wc` puis on la kill apres 
avoir recupéré l'id par la commande `>ps` par la commande `>kill -9 67543`(ici notre id etait 67543)

## Question 5
>On souhaite mesurer le temps d’exécution des commandes.

Pour ce faire on fait appel à la bibliothèque `#include <time.h>`  
/!\pour prendre la mesure de fin il ne faut pas le prendre au niveau du fils sinon la mesure est faussé ,il faut le mettre apres le wait.

