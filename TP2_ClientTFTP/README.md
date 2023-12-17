# TP2 Client TFTP
## Objectif  
L'objectif est de créer un programme client qui doit être capable d’échanger des fichiers à l’aide
du protocole TFTP (Trivial File Transfert Protocol).
## Teste du Serveur 
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

Le client demande une requete de lecture au serveur:  

*2ème communication :*  
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
### puttftp
![Capture Wireshark des 4 communications relevées lors du puttftp.](https://raw.githubusercontent.com/Nathf3/TP_synthese/main/TP2_ClientTFTP/photo/putrequette.png)  
Comme pour le gettftp, on observe les informations concernant les différentes communications entre le client et le serveur.  
Ici, l'objectif est de déposer un fichier présent chez le client sur le serveur.
![1ère communication](https://raw.githubusercontent.com/Nathf3/TP_synthese/main/TP2_ClientTFTP/photo/putrequet1.png)  
Le client (53405) commence par communiquer au serveur le nom du fichier qu'il souhaite transférer.
![2ème communication](https://raw.githubusercontent.com/Nathf3/TP_synthese/main/TP2_ClientTFTP/photo/putrequet2.png)
Le serveur répond par un accusé de réception de la requête.
![3ème communication](https://raw.githubusercontent.com/Nathf3/TP_synthese/main/TP2_ClientTFTP/photo/putrequet3.png)
Le client envoie le contenu du fichier à transférer.
![4ème communication](https://raw.githubusercontent.com/Nathf3/TP_synthese/main/TP2_ClientTFTP/photo/putrequet4.png)
Le serveur accuse réception du contenu du fichier.
