//
// Created by themegatb on 1/3/17.
//

#include "../include/Plugin.hpp"

using namespace std;

void Plugin::process(json datagram) {
    if (datagram == NULL ||
        !datagram.is_object() ||
        datagram.count("action") == 0) {
        err("Incoming request is missing action field.");
        return;
    }
    if (!datagram["action"].is_string()) {
        err("Incoming request is having a wrong type in action field.");
        return;
    }
    string action = datagram["action"];

    Channel *channel = NULL;
    if (datagram.count("channel") && datagram["channel"].is_array()) {
        vector<int> raw_channel = datagram["channel"];
        Channel c(&raw_channel);
        channel = &c;
    }

    this->callback(this, action, channel, datagram);
}

Plugin::Plugin(pluginCallback cb, pluginInit init, string name, string version) {
    this->initFunc = init;
    this->callback = cb;
    this->name = name;
    this->version = version;
    this->initialized = false;
}

string Plugin::getDescriptor() {
    return this->name + " Plugin v" + this->version;
}

int Plugin::init() {
    if (this->initialized) {
        warn("Plugin " + this->name + " is already initialized");
        return 1;
    }

    this->initialized = true;
    return this->initFunc(this);
}

//void Plugin::setSocket(UDPSocket *socket) {
//    this->sock = socket;
//}
