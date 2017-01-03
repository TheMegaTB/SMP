//
// Created by themegatb on 1/3/17.
//

#include "../include/Plugin.hpp"

using namespace std;

void Plugin::process(json datagram) {
    string action = datagram["action"];
    vector<int> raw_channel = datagram["channel"];
    Channel channel = Channel(raw_channel);
    json payload = datagram["payload"];


    // TODO: Add possibility to automagically answer read requests in case no readCB got defined

    if (action == "read") {
        this->readCB(channel, payload);
    } else if (action == "write") {
        this->writeCB(channel, payload);
    } else if (action == "state") {
        //TODO Ignore (?)
        // Might be useful for persistence services
    }
}

Plugin::Plugin(readCallback r, writeCallback w) {
    this->readCB = r;
    this->writeCB = w;
}