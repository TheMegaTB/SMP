//
// Created by themegatb on 1/3/17.
//

#include "Plugin.hpp"

using namespace std;

void Plugin::processJSON(json datagram) {
    string action = datagram["action"];
    vector<int> raw_channel = datagram["channel"];
    Channel channel = Channel(raw_channel);
    json payload = datagram["payload"];
//    Channel channel = Channel(1, 1, 1);


    // TODO: Add possibility to automagically answer read requests in case no readCB got defined

    if (action == "read") {
        (this->*readCB)(channel);
    } else if (action == "write") {
        (this->*writeCB)(channel, payload);
    } else if (action == "state") {
        //TODO Ignore (?)
        // Might be useful for persistence services
    }
}
