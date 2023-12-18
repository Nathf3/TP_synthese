//
// Created by ghostrider on 11/12/23.
//

#ifndef TP2_CLIENTTFTP_TFTP_H
#define TP2_CLIENTTFTP_TFTP_H
#endif //TP2_CLIENTTFTP_TFTP_H

//Include//
#include <stdio.h>
#include <string.h>
#include "sys/types.h"
#include "sys/socket.h"
#include "netdb.h"
#include <stdlib.h>

//define
#define RRQ_Opcode '\x01'
#define WRQ_Opcode '\x02'
#define DATA_Opcode '\x03'
#define ACK_Opcode '\x04'
#define ACK_Block '\x01'
#define Null_Byte '\0'
#define RRQ_Mode "netascii"
#define WRQ_Mode "netascii"

#define MAX_BUFFER_SIZE 512
//Function//
void checkArgumentNumbers(int numberOfArgument);
struct addrinfo *get_address_of_server(char *servername,char *port);
int Read_Request(char * file_name,int sock,struct addrinfo * client);//read request function
void Acknowledgment(int block,int sock,struct sockaddr_storage *server_addr, socklen_t server_addr_len);//send Acknowledgment
