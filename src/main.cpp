#include <iostream>
#include <thread>

#include "networking/udpsocket.hpp"

void server(UDPSocket sock) {
    sock.send();
}

void client(UDPSocket sock) {
    sock.recv();
}

int main() {
    UDPSocket sock = UDPSocket("224.0.0.1", 1234);
    std::thread srv(server, sock);     // spawn new thread that calls foo()
    std::thread clnt(client, sock);    // spawn new thread that calls bar(0)

    std::cout << "main, foo and bar now execute concurrently...\n";

    // synchronize threads:
//    srv.join();                  // pauses until first finishes
    clnt.join();               // pauses until second finishes

//    srv.terminate();
    sock.close();

    std::cout << "foo and bar completed.\n";

    return 0;
}