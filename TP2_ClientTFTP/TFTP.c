
#include "TFTP.h"
int main(int argc, char * argv[ ]){
    checkArgumentNumbers( argc);//verification of number of arguments
    //storage of arguments
    char * file = argv[1];
    char * servername = argv[2];
    char * port = argv[3];
    //buffer init
    char bufferServiceName[128] = {0};
    char bufferHostname[128] = {0};

    printf("Trying to get %s from %s on port %s\n", file, servername,port);
    struct addrinfo * client = get_address_of_server(servername, port);
    int sock=socket( client->ai_family , client->ai_socktype, client->ai_protocol );
    if(sock < 0) { //control error
        perror("socket error");
        exit(EXIT_FAILURE );
    }

    getnameinfo(client -> ai_addr,client -> ai_addrlen,bufferHostname,128,bufferServiceName,128,NI_NUMERICHOST | NI_NUMERICSERV);
    printf("server : %s:%s\n",bufferHostname, bufferServiceName);//test display client ip
    int numberOfCaracterSend=sendto(sock,"00 01 ones256 neta scii 0",128,0,client->ai_addr,client ->ai_addrlen);
   //  a lettre en forme le 00 01 onese25 neta scii etc

    if(numberOfCaracterSend==-1) exit(EXIT_FAILURE);
    printf("number of caracter send :%d \n",numberOfCaracterSend);
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
void checkArgumentNumbers(int numberOfArgument){// verification that the number of argument are good (4)
    if(numberOfArgument > 4){
        fprintf(stderr,"Too many argument \ngettftp [file_name] [server_name] [port_number]");
        exit(EXIT_FAILURE);
    }
    if(numberOfArgument<4){
        fprintf(stderr,"Not enough argument \nHelp: gettftp [file_name] [server_name] [port_number]");
        exit(EXIT_FAILURE);
    }
}