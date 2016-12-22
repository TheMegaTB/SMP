//
// Created by themegatb on 12/22/16.
//

#ifndef SMARTHOME_NETWORKING_H
#define SMARTHOME_NETWORKING_H

#include <netinet/in.h>
#include <cstring>
//#include <stdio.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <netdb.h>
//#include <stdlib.h>
//#include <unistd.h>
//#include <string.h>
//#include <errno.h>

class UDPSocket {
    int sock;
    struct ip_mreq command;
    sockaddr_in multicast_addr;
    int port;
public:
    UDPSocket(in_addr_t multicast_group, uint16_t port);
    void send();
    void recv();
};


#endif //SMARTHOME_NETWORKING_H
