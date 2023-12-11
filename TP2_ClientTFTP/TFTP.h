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

struct addrinfo *get_address_of_server(char *servername,char *port);