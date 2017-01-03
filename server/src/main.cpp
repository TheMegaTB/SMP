#include <iostream>
#include <thread>

#include "json.hpp"
#include "Channel.hpp"
#include "PluginHandler.hpp"

// for convenience
using json = nlohmann::json;

//#include "../smartHomePlugin/include/udpsocket.hpp"
//#include "plugin_framework/SafeQueue.hpp"
//#include "plugin_framework/EventQueue.hpp"
//#include "plugin_framework/Channel.hpp"
//#include "plugin_framework/Plugin.hpp"

// PLUGIN CODE
void wc(Channel c, json p) {
    std::cout << "WRITE" << std::endl;
//    std::cout << c.getAddressAsString() << std::endl;
    std::cout << p.dump(4) << std::endl;
};

json rc(Channel c, json p) {
    std::cout << "READ" << std::endl;
//    std::cout << c.getAddressAsString() << std::endl;
    std::cout << p.dump(4) << std::endl;
    json j;
    return j;
};
// PLUGIN CODE END

//void server(UDPSocket sock, InterruptHandle *handle) {
//    json request = {
//            {"action",  "read"},
//            {"channel", {1, 2, 1}},
//            {"payload", {{"something", "there"}}}
//    };
//    string str_request = request.dump();
//    while (!(*handle).isInterrupted()) {
//        sock.send(str_request);
//        sleep(1);
//    }
//}
//
//void pluginHandler(SafeQueue<json> *q, InterruptHandle *handle) {
//    EventQueue<json> e = EventQueue<json>(q);
//    Plugin p = Plugin(&rc, &wc);
//    e.addObserver(&p);
//    while (!(*handle).isInterrupted())
//        e.observeOnce();
//}
//
//void udpReceptionThread(UDPSocket sock, SafeQueue<json> *udpRecv, InterruptHandle *handle) {
//    std::string data;
//    while (!(*handle).isInterrupted()) {
//        if (sock.recv(&data, 2000) > 0)
//            udpRecv->add(json::parse(data)); // TODO Try/Catch for parsing
//    }
//}

int main() {

    PluginHandler ph("224.0.0.1", 1234);

    Plugin p = Plugin(&rc, &wc);
    ph.addPlugin(&p);

    while (1) {
        ph.receiveData(2000);
        ph.processData();
    }

//    SafeQueue<json> q;
//    UDPSocket sock = UDPSocket("224.0.0.1", 1234);
//    InterruptHandle handle = InterruptHandle();

//    std::thread t1(udpReceptionThread, sock, &q, &handle);
//    std::thread t2(pluginHandler, &q, &handle);
//    std::thread srv(server, sock, &handle);

//    sleep(4);
//    handle.interrupt();
//    t1.join();
//    t2.join();
//    srv.join();

//    sock.close();

    return 0;
}