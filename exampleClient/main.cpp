#include "UDPSocket.hpp"

#include "json.hpp"

using json = nlohmann::json;

int main() {
    json j = {
            {"action",  "read"},
            {"channel", {1, 2, 1}},
            {"payload", {{"something", "there"}}}
    };
    UDPSocket sock("224.0.0.1", 1234);
    while (1) {
        sock.send(j.dump());
        sleep(1);
    }
}