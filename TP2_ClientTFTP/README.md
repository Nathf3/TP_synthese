# TP2 Client TFTP
## Objectif  
L'objectif est de créer un programme client qui doit être capable d’échanger des fichiers à l’aide
du protocole TFTP (Trivial File Transfert Protocol).
## Etape 0 - Test du Serveur 
>Pour tester notre client nous avon sdécidé d'utiliser le serveur présent sur moodle

Pour lancer le serveur, il faut le télécharger et ouvrir le fichier `./go.sh` .  
Dans un autre Terminal on lance la commande `tftp 127.0.0.1 1069` et on lance wireshark.
### Test du téléchargement du fichier ones256  
On utilise pour ce faire la commande `get ones256` et on observe le résulat sur wireshark:   
![Capture Wireshark des 3 communications relevées lors du gettftp.](https://github.com/Nathf3/TP_synthese/blob/main/TP2_ClientTFTP/photo/getrequest.png)  
On observe donc 3 communications entre serveur et client.  

**1ère communication :**  

![1ère communication.](https://raw.githubusercontent.com/Nathf3/TP_synthese/main/TP2_ClientTFTP/photo/getrequest1.png)  
On a donc pour la première communication le paquet suivant :`00 01 6f 6e 65 73 32 35 36 00 6e 65 74 61 73 63 69 69 00`  
D'apres la documentation on sait :  
![Documentation.](https://github.com/Nathf3/TP_synthese/blob/main/TP2_ClientTFTP/photo/tftp%20protocole.png)
![Documentation.](https://github.com/Nathf3/TP_synthese/blob/main/TP2_ClientTFTP/photo/read%20and%20write.png)  
- Opcode : 00 01(1 requete de lecture)
- Filename : 6f 6e 65 73 32 35 36 00 (nom du fichier ones256)
- 0 : null octet de séparation
- Mode : 6e 65 74 61 73 63 69 69(mode qui est du netascii)
- 0 : Fin

Le client envoie une requête de lecture de fichier au serveur. 

**2ème communication :**  
![2ème communication.](https://raw.githubusercontent.com/Nathf3/TP_synthese/main/TP2_ClientTFTP/photo/getrequest2.png)  
D'après la documentation :  
![2ème communication.](https://github.com/Nathf3/TP_synthese/blob/main/TP2_ClientTFTP/photo/data.png)  

On a :  
- Opcode :00 03 (paquet de data avec un numéro de bloc et des données)
-Block : 00 01 (block numéro 1)
-Data : FF...FF (on a bien les 257 caractère ÿ codé en netascii)

Le serveur répond à la requête en envoyant le contenu du fichier demandé au client.  

**3ème communication :**  
![3ème communication.](https://raw.githubusercontent.com/Nathf3/TP_synthese/main/TP2_ClientTFTP/photo/guetrequest3.png)  
Enfin, cette troisième communication est un accusé de réception des données demandées par le client, comme le montre la documentation:  
![3ème communication.](https://github.com/Nathf3/TP_synthese/blob/main/TP2_ClientTFTP/photo/ack.png)  
On a :  
- Opcode : 0004(pour ACK)
- Block : 0001 (pour le numéro de block reçus )

On sait donc comment fonctionne le protocole TFTP pour le téléchargement de fichier :  
1. le client envoie au serveur  une requête de lecture avec le mode de lecture . 
2. Le serveur répond par l'envoi des données du fichier.
3. Le client envoie un accusé de réception.

### Test du téléversement du fichier ones256 
On utilise pour ce faire la commande `put ones256` et on observe le résulat sur wireshark.
![Capture Wireshark des 3 communications relevées lors du gettftp.](https://github.com/Nathf3/TP_synthese/blob/main/TP2_ClientTFTP/photo/putrequest.png)  
Cette fois, la communication s'effectue en 4 étapes : 

**1ère communication :**  
![Capture Wireshark des 3 communications relevées lors du gettftp.](https://github.com/Nathf3/TP_synthese/blob/main/TP2_ClientTFTP/photo/putrequest1.png)  
Le client envoie une demande d'écriture pour le fichier ones256 en "netascii" au serveur.  

**2ème communication :**  
![Capture Wireshark des 3 communications relevées lors du gettftp.](https://github.com/Nathf3/TP_synthese/blob/main/TP2_ClientTFTP/photo/putrequest2.png)  
Le serveur répond par un accusé de réception de la requête, autorisant l'envoi par le client des données à stocker dans le fichier.  

**3ème communication :**  
![Capture Wireshark des 3 communications relevées lors du gettftp.](https://github.com/Nathf3/TP_synthese/blob/main/TP2_ClientTFTP/photo/putrequest3.png)  
Le client envoie le contenu du fichier en netascii au serveur.  

**4ème communication :**  
![Capture Wireshark des 3 communications relevées lors du gettftp.](https://github.com/Nathf3/TP_synthese/blob/main/TP2_ClientTFTP/photo/putrequest4.png)  
Le serveur répond par un accusé de réception des données.  

On sait donc comment fonctionne le protocole TFTP pour le téléversement de fichier :  
1. le client envoie au serveur une requête d'écriture avec le mode de lecture. 
2. Le serveur répond par un accusé de réception.
2. Le client envoie les données du fichier.
3. Le serveur répond par un accusé de réception.

## Etape 1 - Recupération des arguments passés à la ligne de commande des programmes pour obtenir les informations de requêtes  
Pour récupérer les arguments passés en ligne de commande, on vérifie que le nombre d'argument est correct avec la fonction `void checkArgumentNumbers(int numberOfArgument)`.  
Puis, on stocke les différents arguments dans des pointeurs :  
```ruby    
checkArgumentNumbers( argc);//verification of number of arguments
//storage of arguments
char * file = argv[1];
char * servername = argv[2];
char * port = argv[3];
```
## Etape 2 - Appel de `getaddrinfo()` pour obtenir l'adresse du serveur  

Pour ce faire il faut spécifier le type d'adresse, le datagramme et le type de protocole dans notre cas UDP.
Ensuite, on met les arguments dans la fonction est on récupère le résultat dans un pointeur.

```ruby
struct addrinfo * result;
struct addrinfo hints;
memset(&hints,0,sizeof(struct addrinfo));//allocation of memory for the struct 
hints.ai_family = AF_INET; //IPv4
hints.ai_socktype = SOCK_DGRAM;// datagram socket
hints.ai_protocol = IPPROTO_UDP;
hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
int  s= getaddrinfo(servername,port,&hints,&result);
if (s != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
    exit(EXIT_FAILURE);
}
return result;
```
On utilise le code suivant pour vérifier que notre code fonctionne comme souhaité :  
```ruby
getnameinfo(client -> ai_addr,client -> ai_addrlen,bufferHostname,128,bufferServiceName,128,NI_NUMERICHOST | NI_NUMERICSERV);
printf("server : %s:%s\n",bufferHostname, bufferServiceName);//test display client ip
```   
## Etape 3 - Réservation d’un socket de connexion vers le serveur  
La réservation de socket se fait de la manière suivante :
```ruby
int sock=socket( client->ai_family , client->ai_socktype, client->ai_protocol );
if(sock < 0) { //control error
fprintf(stderr,"socket error");
exit(EXIT_FAILURE );
}
```

## Etape 4 - Réalisation de gettftp  
### 4.a Construction d’une requête en lecture (RRQ) et envoi au serveur.  
Comme on l'a vu précédemment, il faut que notre requête contienne un Opcode sur 2 octets, le nom du fichier, 1 octet de séparation, le mode, puis l'octet Null pour marquer la fin de la requête.
On définit donc ces octets en ascii car la requête s'effectue dans ce format :  
```ruby
#define RRQ_Opcode '\x01'
#define WRQ_Opcode '\x02'
#define DATA_Opcode '\x03'
#define ACK_Opcode '\x04'
#define ACK_Block '\x01'
#define Null_Byte '\0'
#define RRQ_Mode "netascii"
#define WRQ_Mode "netascii"
```
On crée notre message d'envoi :  
`snprintf(RRQ_request_msg, sizeof(RRQ_request_msg), "%c%c%s%c%s%02x", Null_Byte,RRQ_Opcode, file_name, Null_Byte, RRQ_Mode, Null_Byte);`  
Puis on effectue l'envoi :
`int numberOfCaracterSend=sendto(sock,RRQ_request_msg,message_size,0,client->ai_addr,client ->ai_addrlen);`
On obtient bien le résultat souhaité :
```ruby
00 01 6f 6e 65 73 ..ones  
32 35 36 00 6e 65 256.neta
74 61 73 63 69 69 scii
00                .
```
### 4.b Réception d’un fichier constitué d’un seul paquet de données (DAT) et son acquit-tement (ACK)  

