#include <iostream>
#include <thread>

#include "json.hpp"

// for convenience
using json = nlohmann::json;

#include "networking/udpsocket.hpp"
#include "SafeQueue.hpp"


void server(UDPSocket sock) {
    while (1) {
        sock.send("I'm different!");
        sleep(1);
    }
}

void client(UDPSocket sock) {
    for (int i = 10; i > 0; --i) {
        std::cout << sock.recv() << std::endl;
    }
}

void threadTest(std::string name, SafeQueue<std::string> &q) {
    while (1) {
        std::cout << name << q.take() << std::endl;
        sleep(1);
    }
}

int main() {

    SafeQueue<std::string> q;
    std::string prefix = "Meow ";
    for (int i = 0; i < 10; ++i) {
        std::ostringstream oss;
        oss << prefix << i;
        q.add(oss.str());
    }

    threadTest("Main thread", q);

    std::thread t1(threadTest, "T1", q);
    std::thread t2(threadTest, "T2", q);

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