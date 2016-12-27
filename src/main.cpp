#include <iostream>
#include <thread>

#include "json.hpp"

// for convenience
using json = nlohmann::json;

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

    json j = {
        {"action", "read"},
        {"channel", 1},
        {"payload", {}}
    };


    std::cout << j.dump(4) << std::endl;

    UDPSocket sock = UDPSocket("224.0.0.1", 1234);
    std::thread srv(server, sock);
    std::thread clnt(client, sock);

    clnt.join();
    srv.detach();

    sock.close();

    return 0;
}