//
// Created by themegatb on 1/3/17.
//

#include "../include/Plugin.hpp"

using namespace std;

void Plugin::process(json datagram) {
    string action = datagram["action"];
    vector<int> raw_channel = datagram["channel"];
    Channel channel = Channel(&raw_channel);
    json payload = datagram["payload"];

    // TODO: Add possibility to automagically answer read requests in case no readCB got defined

    this->callback(action, channel, payload);

//    if (action == "read") {
//        this->readCB(channel, payload);
//    } else if (action == "write") {
//        this->writeCB(channel, payload);
//    } else if (action == "state") {
//        TODO Ignore (?)
//         Might be useful for persistence services
//    }
}

Plugin::Plugin(pluginCallback cb, string name, string version) {
//    this->readCB = r;
//    this->writeCB = w;
    this->callback = cb;
    this->name = name;
    this->version = version;
}

string Plugin::getDescriptor() {
    return this->name + " Plugin v" + this->version;
}
