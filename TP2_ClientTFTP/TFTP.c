
#include "TFTP.h"
int main(int argc, char * argv[ ]){
    checkArgumentNumbers( argc);//verification of number of arguments
    //storage of arguments
    char * file_name = argv[1];
    char * servername = argv[2];
    char * port = argv[3];
    //buffer init
    char bufferServiceName[128] = {0};
    char bufferHostname[128] = {0};

    printf("Trying to get %s from %s on port %s\n", file_name, servername,port);
    struct addrinfo * client = get_address_of_server(servername, port);
    //socket build
    int sock=socket( client->ai_family , client->ai_socktype, client->ai_protocol );
    if(sock < 0) { //control error
        fprintf(stderr,"socket error");
        exit(EXIT_FAILURE );
    }
    //test display client ip
    getnameinfo(client -> ai_addr,client -> ai_addrlen,bufferHostname,128,bufferServiceName,128,NI_NUMERICHOST | NI_NUMERICSERV);
    printf("server : %s:%s\n",bufferHostname, bufferServiceName);



    int NumberOfCaracterSend=Read_Request(file_name,sock,client);
    printf("number of caracter send :%d \n",NumberOfCaracterSend);
    return EXIT_SUCCESS;
}

struct addrinfo * get_address_of_server(char * servername,char * port){
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

int Read_Request(char * file_name,int sock,struct addrinfo * client){//read request function

    //construction and send of RRQ
    int message_size = 2+ strlen(file_name) +1+ strlen(RRQ_Mode)+1; // size of msg

    char RRQ_request_msg[message_size];
    // construction of RRQ
    snprintf(RRQ_request_msg, sizeof(RRQ_request_msg), "%c%c%s%c%s%02x", Null_Byte,RRQ_Opcode, file_name, Null_Byte, RRQ_Mode, Null_Byte);
    //envoie du RRQ
    int numberOfCaracterSend=sendto(sock,RRQ_request_msg,message_size,0,client->ai_addr,client ->ai_addrlen);

    if(numberOfCaracterSend==-1) {
        fprintf(stderr,"error send RRQ");
        exit(EXIT_FAILURE);
    }
    return numberOfCaracterSend;
}