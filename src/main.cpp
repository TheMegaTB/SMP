#include <iostream>
#include <thread>

#include "json.hpp"

// for convenience
using json = nlohmann::json;

#include "networking/udpsocket.hpp"
#include "SafeQueue.hpp"
#include "EventQueue.hpp"

class RandomObserver : public Observer<std::string> {
public:
    void process(std::string event) override {
        std::cout << "RO | " << event << std::endl;
    };
};

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

void threadTest(SafeQueue<std::string> *q) {
    EventQueue<std::string> e = EventQueue<std::string>(q);
    RandomObserver *s = new RandomObserver;
    e.addObserver(s);
    while (1)
        e.observeOnce();
}

int main() {
    SafeQueue<std::string> q;
    std::string prefix = "Event ";

    std::thread t1(threadTest, &q);
    t1.detach();

    std::cout << "HEY THERE" << std::endl;
    for (int i = 0; i < 10; ++i) {
        std::ostringstream oss;
        oss << prefix << i;
        q.add(oss.str());
        sleep(1);
    }

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