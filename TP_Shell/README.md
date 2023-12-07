# TP de Synthèse – Ensea in the Shell
L'objectif de ce TP est de réaliser un micro shell enseash capable d'exécuter des commandes
et d'afficher des informations sur leurs exécutions.

## Question 1
> On souhaite écrire un message de bienvenue pour démarer le programme.  

Pour ce faire, on utilise la commande :  
```ruby
#define welcome_message "Welcome in the ENSEA Shell. \nTo exit, type 'exit'\n"
#define MAX_INPUT_LENGTH 100
write (STDOUT_FILENO,welcome_message, strlen(welcome_message));
```
Avec STDOUT_FILENO = 0 (sortie), on obtient :
```
Welcome in the ENSEA Shell. 
To exit, type 'exit'
enseash %
```
## Question 2
> On souhaite maintenant récupérer une commande de notre terminal ,l'executer puis revenir au prompteur de base `enseash %`

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
On commence par prendre une mesure de temp avant le fork 
```ruby 
double time_spent = 0.0;
clock_t begin = clock(); 
```
puis on prend une mesure apres le wait du processus père:
```ruby
wait(&status);
clock_t end = clock();   //End of the execution time measurement
time_spent += 1000 * (double)(end - begin) / CLOCKS_PER_SEC;
```
puis on affiche en ms apres chaque commande et on obtient:
```ruby
enseash % hostname
d297-pc2.smbedu.ensea.fr
enseahsh [exit : 0 | 0.084 ms] % 
```
## Question 6
>On souhaite réaliser des commandes complexes et récupperer les arguments de celle-ci

Pour ce faire on vas utiliser la fonction strtok qui permet de stocker dans un pointeur des chaine de caractères séparer par un séparateur dans notre cas on utilise l'espace.
Pour utiliser la fonction strtok on commence à lui faire un premier appel ou on lui donne la chaine de charactère à analiser puis on lui spécifie le séparateur .
Une fois la première appel pour récuperer les autres mots il faut réappeller la fonction cette fois avec NULL comme argument à la place de la chaine de caractère et toujour le séparateur .
```ruby
int i=0;
char* command[10];//initialisation of the pointer of a table
char* mot = strtok(buffer," ");//start strtok to put the first word in the pointer mot 

while(mot!=NULL ) {//while loop to store all the word 
    command[i]=mot;
    i++;
    mot = strtok(NULL," ");//after the first call the next need to be call like that to get all word 
}
command[i]=NULL;// at the end of the table we put NULL to end the command 
```
Il ne faut pas oublier de mettre NULL apres la dernière instruction de command dans le tableau pour que par la suite la fonction ` execvp(command[0],command);` comprène jusqu'ou vas la commande.
Pour que cela fonctionne il faut remplacer `execlp()` par `execvp()`.
En effet "l" de execlp() signifie que tous les arguments sont passés un par un en arguments de execl*, en terminant par (char*)NULL .
Or nous on souhaite envoyer notre tableau donc on à besoin de "v" les arguments sont passés en une fois dans un (char **) dont le dernier élément est (char*)NULL.
Apres teste on obtient le résultat suivant:
```ruby
enseash % hostname -i
127.0.1.1
```

## Question 7
>On souhaite mesurer le temps d’exécution des commandes.

Nous n'avons pas pus finir cette question mais notr idée etait de récuperer le reultat d'execution de la commande d'ouvrir /crrer un fichier text avec open et creat et de détecter les caractères < et > dans les chaines de caractère du tableau commande pour choisir entre ouverture et ecriture ou ouverture et copy .

