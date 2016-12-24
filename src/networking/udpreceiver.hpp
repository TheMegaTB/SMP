//
// Created by themegatb on 12/24/16.
//

#ifndef SMARTHOME_UDPRECEIVER_HPP
#define SMARTHOME_UDPRECEIVER_HPP

class UDPReceiver {
    int socket_descriptor;
    struct sockaddr_in address;
public:
    UDPReceiver(in_addr_t multicast_group, uint16_t port);
//    void recv();
};

#endif //SMARTHOME_UDPRECEIVER_HPP
