#include "UDPSocket.hpp"

#include "json.hpp"

using json = nlohmann::json;

int main() {
    json read = {
            {"action",  "read"},
            {"channel", {1, 2, 1}},
            {"payload", {{"something", "there"}}}
    };
    json write = {
            {"action",  "write"},
            {"channel", {1, 2, 1}},
            {"payload", {{"something", "there"}}}
    };
    UDPSocket sock("224.0.0.1", 1337);
    sock.send(write.dump());
//    while (1) {
        sock.send(read.dump());
//        sleep(1);
//    }
}