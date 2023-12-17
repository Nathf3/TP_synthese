# TP2 Client TFTP
## Objectif  
L'objectif est de créer un programme client qui doit être capable d’échanger des fichiers à l’aide
du protocole TFTP (Trivial File Transfert Protocol).
## Etape 0 - Teste du Serveur 
>Pour tester notre client nous avon sdécidé d'utiliser le serveur présent sur moodle

Pour lancer le serveur il faut le télécharger et ouvrir le fichier `./go.sh` .  
Dans un autre Terminal on lance la commande `tftp 127.0.0.1 1069` et on lance wireshark.
### Teste du téléchargement du fichier ones256  
On utilise pour ce faire la commande `get ones256` et on observe le résulat sur wireshark:   
![Capture Wireshark des 3 communications relevées lors du gettftp.](https://github.com/Nathf3/TP_synthese/blob/main/TP2_ClientTFTP/photo/getrequest.png)  
On observe donc 3 communications entre serveur et client.  

**1ère communication :**  

![1ère communication.](https://raw.githubusercontent.com/Nathf3/TP_synthese/main/TP2_ClientTFTP/photo/getrequest1.png)  
On a donc pour la première communication le pacquet suivant :`00 01 6f 6e 65 73 32 35 36 00 6e 65 74 61 73 63 69 69 00`  
D'apres la documentation on sait :  
![Documentation.](https://github.com/Nathf3/TP_synthese/blob/main/TP2_ClientTFTP/photo/tftp%20protocole.png)
![Documentation.](https://github.com/Nathf3/TP_synthese/blob/main/TP2_ClientTFTP/photo/read%20and%20write.png)  
- Opcode : 00 01(1 requete de lecture)
- Filename : 6f 6e 65 73 32 35 36 00 (nom du fichier ones256)
- 0 : null octet de séparation
- Mode : 6e 65 74 61 73 63 69 69(mode qui est du netascii)
- 0 : Fin

Le client demande une requete de lecture au serveur. 

**2ème communication :**  
![2ème communication.](https://raw.githubusercontent.com/Nathf3/TP_synthese/main/TP2_ClientTFTP/photo/getrequest2.png)  
D'apres la documentation :  
![2ème communication.](https://github.com/Nathf3/TP_synthese/blob/main/TP2_ClientTFTP/photo/data.png)  

On a :  
- Opcode :00 03 (paquet de data avec un numéro de bloc et des données)
-Block : 00 01 (block numéro 1)
-Data : FF...FF (on a bien les 257 caractère ÿ codé en netascii)

Le serveur renvoie donc les donnée au client.  

**3ème communication :**  
![3ème communication.](https://raw.githubusercontent.com/Nathf3/TP_synthese/main/TP2_ClientTFTP/photo/guetrequest3.png)  
Enfin, cette troisième communication est un accusé de réception comme le montre la documentation:  
![3ème communication.](https://github.com/Nathf3/TP_synthese/blob/main/TP2_ClientTFTP/photo/ack.png)  
On a :  
- Opcode : 0004(pour ACK)
- Block : 0001 (pour le numéro de block reçus )

On sait donc comment fonctionne le protocole TFTP pour le téléchargement de fichier :  
1. le client demande au serveur de une requête de lecture avec le mode de lecture . 
2. Le serveur répond par l'envoie des données du fichier.
3. Le client envoie un accusé de réception.

### Teste du téléversement du fichier ones256 
On utilise pour ce faire la commande `put ones256` et on observe le résulat sur wireshark:   
![Capture Wireshark des 3 communications relevées lors du gettftp.](https://github.com/Nathf3/TP_synthese/blob/main/TP2_ClientTFTP/photo/putrequest.png)  
On observe donc 4 communications entre serveur et client.  

**1ère communication :**  
![Capture Wireshark des 3 communications relevées lors du gettftp.](https://github.com/Nathf3/TP_synthese/blob/main/TP2_ClientTFTP/photo/putrequest1.png)  
Le client envoie une Demande d'écriture pour le ficheir ones256 en netascii au serveur.  

**2ème communication :**  
![Capture Wireshark des 3 communications relevées lors du gettftp.](https://github.com/Nathf3/TP_synthese/blob/main/TP2_ClientTFTP/photo/putrequest2.png)  
Le serveur répond qu'il est daccord avec un accusé de réception.  

**3ème communication :**  
![Capture Wireshark des 3 communications relevées lors du gettftp.](https://github.com/Nathf3/TP_synthese/blob/main/TP2_ClientTFTP/photo/putrequest3.png)  
Le client envoie donc le contenue du fichier en netascii au serveur.  

**4ème communication :**  
![Capture Wireshark des 3 communications relevées lors du gettftp.](https://github.com/Nathf3/TP_synthese/blob/main/TP2_ClientTFTP/photo/putrequest4.png)  
Le serveur renvoie alors qu'il à bien reçus par un accuée de réception.  

On sait donc comment fonctionne le protocole TFTP pour le téléversement de fichier :  
1. le client demande au serveur de une requête d'écirtue avec le mode de lecture . 
2. Le serveur répond par un accusée de réception.
2. Le client répond par l'envoie des données du fichier.
3. Le serveur répond par un accusé de réception.

## Etape 1 - Recupération des arguments passés à la ligne de commande des programmes pour obtenir les informations de requêtes  
Pour récupérer les arguments passés en ligne de commande on vérifie que le nombre d'argument est correcte avec la fonction `void checkArgumentNumbers(int numberOfArgument)`.  
Puis, on stocke les différents arguments dans des pointeurs :  
```ruby    
checkArgumentNumbers( argc);//verification of number of arguments
//storage of arguments
char * file = argv[1];
char * servername = argv[2];
char * port = argv[3];
```
## Etape 2 - Appel de `getaddrinfo(servername,port,&hints,&result)` pour obtenir l'adresse du serveur  


