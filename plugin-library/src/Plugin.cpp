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

Plugin::Plugin(pluginCallback cb, pluginInit init, string name, string version, bool loadConfig) {
    this->loadConfig = loadConfig;
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

    if (this->loadConfig) {
        char *CONFIG_DIR = getenv("CONFIG_DIR");
        string confDir;
        if (CONFIG_DIR == NULL) {
            confDir = ".";
        } else {
            confDir = CONFIG_DIR;
        }

        std::ifstream t(confDir + "/" + this->name + ".json");

        if (!t.good()) {
            err(this->name + " config file not found!");
            return 1;
        }

        json j;

        try {
            t >> j;
        } catch (const std::invalid_argument err) {
            err("Failed to parse " + this->name + " config file");
            return 1;
        }

        this->config = j;
    }

    this->initialized = true;
    return this->initFunc(this);
}
