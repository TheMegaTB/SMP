#include <string>
#include <fstream>

#include <Logger.hpp>
#include <stdlib.h>
#include "Plugin.hpp"
#include "knx.hpp"

using json = nlohmann::json;

class Device {
public:
    virtual void set(json)= 0;
};

class Fixture : public Device {
    eibaddr_t binaryAddr;
    eibaddr_t dimmableAddr;
    bool dimmable;

public:
    void set(json value) override {
        if (value.is_boolean()) {
            switchLight(this->binaryAddr, value);
        } else if (value.is_number_integer() && this->dimmable) {
            dimLight(this->dimmableAddr, value);
        }
    }

    Fixture(json attributes) {
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

std::map<Channel, Device> devices;

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
        Device dev;

        if (device["type"] == "fixture") {
            dev = new Fixture(device["attributes"]);
        } else if (device["type"] == "shutter") {
            dev = new Shutter(device["attributes"]);
        }

        vector<int> channel = device["channel"];
        devices[Channel(channel[0], channel[1], channel[2])] = dev;
    }

    return 0;
}

void callback(string action, Channel c, json p) {
    trace(action);
    trace(c.getAddressAsString());
    trace(p.dump());

//    if (p.is_boolean()) {
//        switchLight("0/4/0", p);
//    } else if (p.is_number_integer()) {
//        dimLight("0/4/2", p);
//    }
};

int init() {
    return readConfig();
}

extern "C" Plugin* load_plugin() {
    return new Plugin(callback, init, "KNX", "0.0.1");
}

extern "C" void unload_plugin(Plugin* p) {
    delete p;
}