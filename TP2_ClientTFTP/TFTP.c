//
// Created by bastduqu54 on 11/12/23.
//
#include <stdio.h>
#include <string.h>
#include "sys/types.h"
#include "sys/socket.h"
#include "netdb.h"
int main(int argc, char *argv[ ]){
    char bufferServiceName[128]={0};
    char bufferHostname[128]={0};

    if(argc != 4){
        perror("Wrong usage : gettftp filename server port \n");
    }
    printf("Trying to get %s from %s on port %s\n", argv[1], argv[2], argv[3]);
    struct addrinfo * result;
    struct addrinfo hints;
    memset(&hints,0,sizeof(struct addrinfo));
    hints.ai_family=AF_INET;
    hints.ai_socktype=SOCK_DGRAM;
    hints.ai_protocol=IPPROTO_UDP;
    getaddrinfo(argv[2],argv[3],&hints,&result);
    getnameinfo(result-> ai_addr,result->ai_addrlen,bufferHostname,128,bufferServiceName,128,NI_NUMERICHOST | NI_NUMERICSERV);
    printf("serveur : %s:%s\n",bufferHostname,bufferServiceName);
    return 0;
}