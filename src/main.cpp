#include <iostream>
#include <thread>

#include "json.hpp"

// for convenience
using json = nlohmann::json;

#include "networking/udpsocket.hpp"
#include "SafeQueue.hpp"
#include "EventQueue.hpp"
#include "InterruptHandle.hpp"
#include "plugin_framework/Channel.hpp"
#include "plugin_framework/Plugin.hpp"

class RandomObserver : public Observer<std::string> {
public:
    void process(std::string event) override {
        std::cout << "RO | " << event << std::endl;
    };
};

void server(UDPSocket sock, InterruptHandle *handle) {
    while (!(*handle).isInterrupted()) {
        sock.send("I'm different!");
        sleep(1);
    }
}

void client(UDPSocket sock) {
    for (int i = 10; i > 0; --i) {
        std::cout << sock.recv() << std::endl;
    }
}

void pluginHandler(SafeQueue<std::string> *q, InterruptHandle *handle) {
    EventQueue<std::string> e = EventQueue<std::string>(q);
    RandomObserver *s = new RandomObserver;
    e.addObserver(s);
    while (!(*handle).isInterrupted())
        e.observeOnce();
}

void udpReceptionThread(UDPSocket sock, SafeQueue<std::string> *udpRecv, InterruptHandle *handle) {
    while (!(*handle).isInterrupted()) {
        udpRecv->add(sock.recv());
    }
}

void jsonTesting() {
    json j = {
            {"action",  "read"},
            {"channel", 1},
            {"payload", {}}
    };

    j["list"] = {"Test1", "Test2", "Test3"};

    std::vector<std::string> test = j["list"];
    for (std::string s : test) {
        std::cout << s << std::endl;
    }

    std::cout << j.dump(4) << std::endl;
}

void wc(Channel c, json p) {
    std::cout << "WRITE" << std::endl;
    std::cout << c.getAddressAsString() << std::endl;
    std::cout << p.dump(4) << std::endl;
};

json rc(Channel c, json p) {
    std::cout << "READ" << std::endl;
    std::cout << c.getAddressAsString() << std::endl;
    std::cout << p.dump(4) << std::endl;
    json j;
    return j;
};

void pluginTesting() {
    Plugin p = Plugin(&rc, &wc);
    json request = {
            {"action",  "read"},
            {"channel", {1,           2, 1}},
            {"payload", {{"something", "there"}}}
    };
//    auto request = R"(
//          {
//            "action": "read",
//            "channel": [1,2,1],
//            "payload": {
//                "something": "there"
//            }
//          }
//        )"_json;
    p.processJSON(request);
}

int main() {
//    jsonTesting();
    pluginTesting();

    SafeQueue<std::string> q;
    UDPSocket sock = UDPSocket("224.0.0.1", 1234);
    InterruptHandle handle = InterruptHandle();

    std::thread t1(udpReceptionThread, sock, &q, &handle);
    std::thread t2(pluginHandler, &q, &handle);
    std::thread srv(server, sock, &handle);

    sleep(4);
    handle.interrupt();
    t1.join();
    t2.join();
    srv.join();

    sock.close();

    return 0;
}