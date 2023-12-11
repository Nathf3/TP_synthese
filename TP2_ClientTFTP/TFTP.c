
#include "TFTP.h"
int main(int argc, char * argv[ ]){
    char * servername = argv[2];
    char * port = argv[3];
    char bufferServiceName[128] = {0};
    char bufferHostname[128] = {0};

    if(argc != 4){
        perror("Wrong usage : gettftp filename server port \n");
    }

    printf("Trying to get %s from %s on port %s\n", argv[1], argv[2], argv[3]);

    struct addrinfo * client = get_address_of_server(servername, port);
    int sock=socket( client->ai_family , client->ai_socktype, client->ai_protocol );
    if(sock < 0) { //control error
        perror("socket error");
        exit(EXIT_FAILURE );
    }

    getnameinfo(client -> ai_addr,client -> ai_addrlen,bufferHostname,128,bufferServiceName,128,NI_NUMERICHOST | NI_NUMERICSERV);
    printf("server : %s:%s\n",bufferHostname, bufferServiceName);//test display client ip
    sendto(sock,bufferHostname,128,);

    return EXIT_SUCCESS;
}

struct addrinfo * get_address_of_server(char * servername,char * port){
    struct addrinfo * result;
    struct addrinfo hints;
    memset(&hints,0,sizeof(struct addrinfo));
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
}
