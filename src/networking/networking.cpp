//
// Created by themegatb on 12/22/16.
//

#include <cstdio>
#include <stdlib.h>
#include <zconf.h>
#include <arpa/inet.h>
#include <string.h>
#include "networking.hpp"

UDPSender::UDPSender(in_addr_t multicast_group, uint16_t port) {
    socket_descriptor = socket (AF_INET, SOCK_DGRAM, 0);
    if (socket_descriptor == -1) {
        perror ("socket()");
        exit (EXIT_FAILURE);
    }
    memset (&address, 0, sizeof (address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = multicast_group;//inet_addr("224.0.0.1");
    address.sin_port = htons (port);
}

void UDPSender::send() {
    while (1) {
        if (sendto( socket_descriptor,
                    "broadcast test (hallo client)",
                    sizeof ("broadcast test (hallo client)"),
                    0,
                    (struct sockaddr *) &address,
                    sizeof (address)) < 0) {
            perror ("sendto()");
            exit (EXIT_FAILURE);
        }
        sleep(1);
    }
}

//void UDPSender::recv() {
//    char message[256];
//    struct sockaddr_in sin;
//    if (recvfrom(this->sock, message, 256, 0,
//                 (struct sockaddr *) &sin, (socklen_t *) sizeof(sin)) == -1) {
//        perror ("recvfrom");
//    }
//}

