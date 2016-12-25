//
// Created by themegatb on 12/24/16.
//

#ifndef UDP_SOCKET_HPP
#define UDP_SOCKET_HPP

class UDPSocket {
    int sock;
    struct ip_mreq command;
    struct sockaddr_in sin;
public:
    UDPSocket(std::string multicast_group, uint16_t port);

    void recv();

    void close();

    void send();
};

#endif //UDP_SOCKET_HPP
