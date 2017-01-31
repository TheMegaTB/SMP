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
        trace("Setting light to " + value.dump());
        if (value.is_boolean()) {
            switchLight(this->binaryAddr, value);
            trace("Set binary value");
        } else if (value.is_number_integer() && this->dimmable) {
            dimLight(this->dimmableAddr, value);
            trace("Set number value");
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
        err("KNX config file not found!");
        return 1;
    }

    json j;

    try {
        t >> j;
    } catch (const std::invalid_argument err) {
        err("Failed to parse KNX config file");
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

void callback(Plugin *context, string action, Channel *c, json raw) {
    if (action == "query") {
        for (pair<const string, Device *> &d : devices) {
            string deviceChannel = d.first;
            json dev = {
                    {"action",     "announce"},
                    {"type",       "fixture"}, // TODO Insert some meaningful value here
                    {"channel",    Channel(deviceChannel).getAddress()},
                    {"attributes", d.second->attributes}
            };
            context->outgoingDatagrams.add(dev.dump());
        }
    } else if (action == "write" && c != nullptr && devices.find(c->getAddressAsString()) != devices.end()) {
        devices[c->getAddressAsString()]->set(raw["payload"]);
    }
};

int init(Plugin *context) {
    return readConfig();
}

extern "C" Plugin* load_plugin() {
    return new Plugin(callback, init, "KNX", "0.0.2");
}

extern "C" void unload_plugin(Plugin* p) {
    delete p;
}