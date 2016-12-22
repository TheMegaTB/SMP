# include <iostream>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
//# include <string>

//# include <unistd.h>
//# include <stdlib.h>
//#include <lib/libsocket/headers/inetclientstream.hpp>

/*
 * This program connects to host:port (usually localhost),
 * closes the socket, reopens it with the same parameters
 * (to show what you may do with sockets... ;), receives
 * a message from the server and sends a message back.
 */
#include "networking/networking.hpp"

int main(void)
{
//    using std::string;
//
//    using libsocket::inet_stream;
//
//    string host = "::1";
//    string port = "1235";
//    string answer;
//
//    answer.resize(32);
//
//    try {
//        libsocket::inet_stream sock(host,port,LIBSOCKET_IPv6);
//
//        sock >> answer;
//
//        std::cout << answer;
//
//        sock << "Hello back!\n";
//
//         sock is closed here automatically!
//    } catch (const libsocket::socket_exception& exc)
//    {
//        std::cerr << exc.mesg;
//    }
//
//    struct sockaddr_in sin;
//    struct hostent *server_host_name;
//    if ((server_host_name = gethostbyname ("244.0.0.1")) == 0) {
//        perror ("gethostbyname");
//        exit (EXIT_FAILURE);
//    }
    UDPSocket sock = UDPSocket(inet_addr("239.0.30.1"), 1234);

    sock.send();
//    sock.recv();
    return 0;
}