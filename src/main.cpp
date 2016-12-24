#include <iostream>
#include <arpa/inet.h>
#include <thread>

#include "networking/udpsender.hpp"
#include "networking/udpreceiver.hpp"

void server() {
    UDPSender sock = UDPSender(inet_addr("224.0.0.1"), 1234);
    sock.send();
}

void client() {
    UDPReceiver sock = UDPReceiver(inet_addr("224.0.0.1"), 1234);
//    sock.recv();
}

int main() {
    std::thread srv(server);     // spawn new thread that calls foo()
    std::thread clnt(client);    // spawn new thread that calls bar(0)

    std::cout << "main, foo and bar now execute concurrently...\n";

    // synchronize threads:
//    srv.join();                  // pauses until first finishes
    clnt.join();               // pauses until second finishes

    std::cout << "foo and bar completed.\n";

    return 0;
}