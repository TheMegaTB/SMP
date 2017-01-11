//
// Created by themegatb on 1/3/17.
//

#include "../include/Plugin.hpp"

using namespace std;

void Plugin::process(string datagram) {
    json data = json::parse(datagram);
    string action = data["action"];
    vector<int> raw_channel = data["channel"];
    Channel channel = Channel(&raw_channel);
    json payload = data["payload"];

    // TODO: Add possibility to automagically answer read requests in case no readCB got defined

    if (action == "read") {
        this->readCB(channel.getAddress(), payload.dump());
    } else if (action == "write") {
        this->writeCB(channel.getAddress(), payload.dump());
    } else if (action == "state") {
        //TODO Ignore (?)
        // Might be useful for persistence services
    }
}

Plugin::Plugin(readCallback r, writeCallback w) {
    this->readCB = r;
    this->writeCB = w;
}