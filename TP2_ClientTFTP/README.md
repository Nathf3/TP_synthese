# TP2 Client TFTP
## 1 : Utilisation des arguments passés à la ligne de commande des programmes gettftp et puttftp pour obtenir les informations de requêtes (serveur et fichier)
>On souhaite observer le trafic pour voir comment fonctionne le protocol tftp 
![Capture Wireshark des 3 communications relevées lors du gettftp.](https://raw.githubusercontent.com/Nathf3/TP_synthese/main/TP2_ClientTFTP/photo/getrequest.png)
On observe trois communications entre le serveur et le client.  
Pour chacunes d'entre elles, on peut voir les ports qui ont communiqué, ainsi que la taille des messages.  
L'objectif de la requête gettftp est de récupérer un fichier présent sur le serveur TFTP.  
![1ère communication.](https://raw.githubusercontent.com/Nathf3/TP_synthese/main/TP2_ClientTFTP/photo/getrequest1.png)  
On capture ci-dessus les informations à propos de la première communication. On observe que le port 40233 (client) a effectué une requête concernant le fichier ones256 au port 1069 (serveur TFTP).  
![2ème communication.](https://raw.githubusercontent.com/Nathf3/TP_synthese/main/TP2_ClientTFTP/photo/getrequest2.png)  
On observe ici les informations concernant la réponse du serveur TFTP à la requête émise par le client.
![3ème communication.](https://raw.githubusercontent.com/Nathf3/TP_synthese/main/TP2_ClientTFTP/photo/getrequest3.png)  
Enfin, cette troisième communication est un accusé de réception du port 40233 qui vient confirmer la bonne réception du fichier ones256 au serveur TFTP.  
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