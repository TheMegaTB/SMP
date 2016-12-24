//
// Created by themegatb on 12/22/16.
//

#ifndef SMARTHOME_NETWORKING_H
#define SMARTHOME_NETWORKING_H

#include <netinet/in.h>
#include <cstring>

class UDPSender {
    int socket_descriptor;
    struct sockaddr_in address;
public:
    UDPSender(in_addr_t multicast_group, uint16_t port);
    void send();
};

#endif //SMARTHOME_NETWORKING_H
