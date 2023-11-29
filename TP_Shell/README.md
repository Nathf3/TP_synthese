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

