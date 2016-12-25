//
// Created by themegatb on 12/24/16.
//
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

#include "udpsocket.hpp"

UDPSocket::UDPSocket(std::string multicast_group, uint16_t port) {
    int loop = 1;
    memset(&sin, 0, sizeof(sin));

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(port);
    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("sock()");
        exit(EXIT_FAILURE);
    }
    /* Mehr Prozessen erlauben, denselben Port zu nutzen */
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &loop, sizeof(loop)) < 0) {
        perror("setsockopt:SO_REUSEADDR");
        exit(EXIT_FAILURE);
    }
    if (bind(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    /* Broadcast auf dieser Maschine zulassen */
    if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_ALL, &loop, sizeof(loop)) < 0) {
        perror("setsockopt:IP_MULTICAST_LOOP");
        exit(EXIT_FAILURE);
    }
    /* Join the broadcast group: */
    command.imr_multiaddr.s_addr = inet_addr(multicast_group.c_str());
    command.imr_interface.s_addr = htonl(INADDR_ANY);
    if (command.imr_multiaddr.s_addr == -1) {
        perror("Invalid multicast adress\n");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &command, sizeof(command)) < 0) {
        perror("setsockopt:IP_ADD_MEMBERSHIP");
    }
}

void UDPSocket::recv() {
    int iter = 0;
    int sin_len;
    while (iter++ < 10) {
        sin_len = sizeof(sin);
        int bytes = 0;
        while (bytes == 0 && ioctl(sock, FIONREAD, &bytes) >= 0)
            sleep(1);
        char message[bytes];
        if (recvfrom(sock, message, 256, 0, (struct sockaddr *) &sin, (socklen_t *) &sin_len) == -1) {
            perror("recvfrom");
        }
        printf("Response #%-2d: %s\n", iter, message);
    }
}

void UDPSocket::send() {
    std::string message = "broadcast test (hallo client)";
    const char *msg = message.c_str();
    while (1) {
        if (sendto(sock, msg, sizeof(message), 0, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
            perror("sendto()");
            exit(EXIT_FAILURE);
        }
        sleep(1);
    }
}

void UDPSocket::close() {
    if (setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, &command, sizeof(command)) < 0) {
        perror("setsockopt:IP_DROP_MEMBERSHIP");
    }
    shutdown(sock, SHUT_RDWR);
}