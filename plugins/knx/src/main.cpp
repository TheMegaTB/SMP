#include <string>
#include <fstream>

#include <Logger.hpp>
#include <stdlib.h>
#include "Plugin.hpp"
#include "knx.hpp"

using json = nlohmann::json;

class Device {
public:
    json attributes;
    virtual void set(json)= 0;
};

class Fixture : public Device {
    eibaddr_t binaryAddr;
    eibaddr_t dimmableAddr;
    bool dimmable;

    json attributes;

public:
    void set(json value) override {
        if (value.is_boolean()) {
            switchLight(this->binaryAddr, value);
        } else if (value.is_number_integer() && this->dimmable) {
            dimLight(this->dimmableAddr, value);
        }
    }

    Fixture(json attributes) {
        this->attributes = attributes;
        string binaryAddr = attributes["binary"];
        readGroupAddr(binaryAddr.c_str(), &this->binaryAddr);
        if (attributes["dimmable"].is_string()) {
            this->dimmable = true;
            string dimmableAddr = attributes["dimmable"];
            readGroupAddr(dimmableAddr.c_str(), &this->dimmableAddr);
        }
    }
};

class Shutter : public Device {
    eibaddr_t shortAddr;
    eibaddr_t longAddr;

public:
    void set(json value) override {
        // TODO Implement some logic for stop, up, down, 3/4 up and 3/4 down
    }

    Shutter(json attributes) {
//        readGroupAddr(attributes["short"].c_str(), &this->shortAddr);
//        readGroupAddr(attributes["long"].c_str(), &this->longAddr);
    }
};

std::map<string, Device *> devices;

int readConfig() {
    char *CONFIG_DIR = getenv("CONFIG_DIR");
    string confDir;
    if (CONFIG_DIR == NULL) {
        confDir = ".";
    } else {
        confDir = CONFIG_DIR;
    }

    std::ifstream t(confDir + "/knx.json");

    if (!t.good()) {
        error("KNX config file not found!");
        return 1;
    }

    json j;

    try {
        t >> j;
    } catch (const std::invalid_argument err) {
        error("Failed to parse KNX config file");
        return 1;
    }

    connectionURL = j["connection"];
    for (auto device : j["devices"]) {
        Device *dev;

        if (device["type"] == "fixture") {
            dev = new Fixture(device["attributes"]);
        } else if (device["type"] == "shutter") {
            dev = new Shutter(device["attributes"]);
        }

        vector<int> channel = device["channel"];
        devices[Channel(channel[0], channel[1], channel[2]).getAddressAsString()] = dev;
    }

    return 0;
}

void callback(Plugin *context, string action, Channel *c, json *p) {
    trace(action);
    if (c != nullptr) trace(c->getAddressAsString());
    if (c != nullptr) trace(p->dump());

    if (action == "query") {
//        typedef std::map<const string, Device *>::iterator it_type;
//        for(auto iterator = devices.begin(); iterator != devices.end(); ++iterator) {
//            // iterator->first = key
//            // iterator->second = value
//            Channel channel = Channel(iterator->first);
//            trace(channel.getAddressAsString());
//        }
        for (pair<const string, Device *> &d : devices) {
//            Channel channel = Channel(d.first);
//            trace(channel.getAddressAsString());
//            json dev = {
//                    {"type",  "write"},
//                    {"channel", Channel(d.first).getAddress()},
//                    {"attributes", d.second->attributes}
//            };
        }
    }

    if (action == "write" && p != nullptr && c != nullptr && devices.find(c->getAddressAsString()) != devices.end()) {
        devices[c->getAddressAsString()]->set(p);
    }

//    if (p.is_boolean()) {
//        switchLight("0/4/0", p);
//    } else if (p.is_number_integer()) {
//        dimLight("0/4/2", p);
//    }
};

int init(Plugin *context) {
    return readConfig();
}

extern "C" Plugin* load_plugin() {
    return new Plugin(callback, init, "KNX", "0.0.1");
}

extern "C" void unload_plugin(Plugin* p) {
    delete p;
}