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

    this->callback(action, channel, payload);
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
    return this->initFunc();
}
