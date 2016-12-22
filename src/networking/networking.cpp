//
// Created by themegatb on 12/22/16.
//

#include <cstdio>
#include <stdlib.h>
#include <libltdl/lt_system.h>
#include "networking.hpp"

UDPSocket::UDPSocket(in_addr_t multicast_group, uint16_t port) {
//    this->multicast_group = multicast_group;
    this->port = port;

//    struct sockaddr_in sin;
    memset(&this->multicast_addr, 0, sizeof(this->multicast_addr));
    this->multicast_addr.sin_family = AF_INET;
    this->multicast_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    this->multicast_addr.sin_port = htons(port);

    if ( (this->sock = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("error creating socket.");
        exit(EXIT_FAILURE);
    }

    int loop = 1;
    if (setsockopt(this->sock, SOL_SOCKET, SO_REUSEADDR, &loop, sizeof(loop)) < 0) {
        perror("setsockopt:SO_REUSEADDR");
        exit(EXIT_FAILURE);
    }

    if (bind(this->sock, (struct sockaddr *)&this->multicast_addr, sizeof(this->multicast_addr)) < 0) {
        perror("couldn't bind socket");
        exit(EXIT_FAILURE);
    }

    loop = 1;
    if(setsockopt(this->sock, IPPROTO_IP, IP_MULTICAST_ALL, &loop, sizeof(loop)) < 0) {
        perror("setsockopt:IP_MULTICAST_LOOP");
        exit(EXIT_FAILURE);
    }

    // Join the multicast group
    this->command.imr_multiaddr.s_addr = multicast_group;
    this->command.imr_interface.s_addr = htonl(INADDR_ANY);
    if (this->command.imr_multiaddr.s_addr == -1) {
        perror("Couldn't join multicast group.");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(this->sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &command, sizeof(command)) < 0) {
        perror("Failed to add IP_MEMBERSHIP");
    }
}

void UDPSocket::send() {
    if (sendto( this->sock,
                "broadcast test (hallo client)",
                sizeof ("broadcast test (hallo client)"),
                0,
                (struct sockaddr *) &this->multicast_addr,
                sizeof (this->multicast_addr)) < 0) {
        perror ("sendto()");
        exit (EXIT_FAILURE);
    }
}

void UDPSocket::recv() {
    char message[256];
    struct sockaddr_in sin;
    if (recvfrom(this->sock, message, 256, 0,
                 (struct sockaddr *) &sin, (socklen_t *) sizeof(sin)) == -1) {
        perror ("recvfrom");
    }
}

