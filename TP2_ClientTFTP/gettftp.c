
#include "gettftp.h"
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

    //send RRQ
    int NumberOfCharacterSend=Read_Request(file_name,sock,client);
    printf("number of character send :%d \n",NumberOfCharacterSend);
    //Acknowledgment(1024,sock,client);


    receive_Data(sock,file_name);

    /*for(int i=0;i<(int)sizeof(bufferreceivefromserver);i++) {
        printf("%d ",bufferreceivefromserver[i]);
    }*/
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
    int numberOfCharacterSend=sendto(sock,RRQ_request_msg,message_size,0,client->ai_addr,client ->ai_addrlen);

    if(numberOfCharacterSend==-1) {
        fprintf(stderr,"error send RRQ");
        exit(EXIT_FAILURE);
    }
    return numberOfCharacterSend;
}

void Acknowledgment(int block,int sock,struct sockaddr_storage *server_addr, socklen_t server_addr_len){//send Acknowledgment
    int len_msg=4;
    char RRQ_request_msg[len_msg];
    sprintf(RRQ_request_msg, "%c%c%c%c",Null_Byte,ACK_Opcode,(int16_t)(block >> 8) , (int16_t)block );

    int numberOfCharacterSend=sendto(sock,RRQ_request_msg,len_msg,0, (struct sockaddr *)server_addr, server_addr_len);
    if(numberOfCharacterSend==-1) {
        fprintf(stderr,"error send RRQ");
        exit(EXIT_FAILURE);
    }
}

void receive_Data(int sock,char * file_name){
    char bufferreceivefromserver[MAX_BUFFER_SIZE+HEAD_SIZE] = {0};//buffer of receive
    struct sockaddr_storage server_addr;
    socklen_t server_addr_len=sizeof(server_addr);

    //first receive_data
    int byte_received= recvfrom(sock,bufferreceivefromserver,sizeof(bufferreceivefromserver),0,(struct sockaddr *)&server_addr,&server_addr_len);
    int byte_receive_total=byte_received-HEAD_SIZE;
    Acknowledgment(1,sock,&server_addr, server_addr_len);
    //open file in writing mode
    FILE *output_file = fopen(file_name, "wb");
    if (output_file == NULL) {
        fprintf(stderr,"error during opening file");
        exit(EXIT_FAILURE);
    }
    // writing data
    fwrite(bufferreceivefromserver + HEAD_SIZE  , sizeof(char), byte_received - HEAD_SIZE, output_file);

    // for more data we check if the number of byte receive is >= than MAX_BUFFER_SIZE. If that's the case, it means that the server will send an other data packet
    while(byte_received>=MAX_BUFFER_SIZE){

        byte_received= recvfrom(sock,bufferreceivefromserver,sizeof(bufferreceivefromserver),0,(struct sockaddr *)&server_addr,&server_addr_len);
        Acknowledgment(bufferreceivefromserver[3],sock,&server_addr, server_addr_len);
        byte_receive_total+=byte_received-HEAD_SIZE;
        // writing data without the 4 begining bytes and set the cursor at the good position
        fseek(output_file, MAX_BUFFER_SIZE*(bufferreceivefromserver[3]-1), SEEK_SET);
        fwrite(bufferreceivefromserver + HEAD_SIZE, sizeof(char), byte_received - HEAD_SIZE, output_file);

        if(byte_received==-1){
            fprintf(stderr,"Error during receive");
            exit(EXIT_FAILURE);
        }
        fclose(output_file);
    }


    printf("data receive: %d ",byte_receive_total);
}