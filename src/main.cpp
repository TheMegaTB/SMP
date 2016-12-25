#include <iostream>
#include <thread>

#include "networking/udpsocket.hpp"

void server(UDPSocket sock) {
    while (1) {
        sock.send();
        sleep(1);
    }
}

void client(UDPSocket sock) {
    for (int i = 10; i > 0; --i) {
        std::cout << sock.recv() << std::endl;
    }
}

int main() {
    UDPSocket sock = UDPSocket("224.0.0.1", 1234);
    std::thread srv(server, sock);
    std::thread clnt(client, sock);

    clnt.join();
    srv.detach();

    sock.close();

    return 0;
}