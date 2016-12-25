#ifndef UDP_SOCKET_HPP
#define UDP_SOCKET_HPP

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

class UDPSocket {
    int sock;
    struct ip_mreq command;
    struct sockaddr_in sin;
public:
    UDPSocket(std::string multicast_group, uint16_t port);

    std::string recv();

    void close();

    int send();
};

#endif //UDP_SOCKET_HPP
