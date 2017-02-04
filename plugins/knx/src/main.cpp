#include <string>

#include "Plugin.hpp"
#include "knx.hpp"

using json = nlohmann::json;

class Device {
public:
    virtual void set(json)= 0;
    virtual json getAttributes()= 0;
    virtual const char* getDeviceType() { return "Device"; }
    virtual string getDeviceName() { return "Unnamed device"; }
};

class Fixture : public Device {
    eibaddr_t binaryAddr;
    eibaddr_t dimmableAddr;
    bool dimmable;

    json attributes;
    string name;

public:
    const char* getDeviceType() override {
        return "Fixture";
    }

    string getDeviceName() override {
        return this->name;
    }

    void set(json value) override {
        if (value.is_boolean()) {
            switchLight(this->binaryAddr, value);
        } else if (value.is_number_integer() && this->dimmable) {
            dimLight(this->dimmableAddr, value);
        }
    }

    json getAttributes() override {
        return this->attributes;
    }

    Fixture(json name, json attributes) {
        this->attributes = attributes;
        this->name = name;
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

    json attributes;
    json name;

public:
    const char* getDeviceType() override {
        return "Shutter";
    }

    string getDeviceName() override {
        return this->name;
    }

    void set(json value) override {
        // TODO Implement some logic for stop, up, down, 3/4 up and 3/4 down
        if (value.is_string()) {
            if (value == "up")
                switchLight(longAddr, 0);
            else if (value == "down")
                switchLight(longAddr, 1);
            else if (value == "stop")
                switchLight(shortAddr, 0);
        }
    }

    json getAttributes() override {
        return this->attributes;
    }

    Shutter(json name, json attributes) {
        this->attributes = attributes;
        this->name = name;

        string shortAddr = attributes["short"];
        string longAddr = attributes["long"];
        readGroupAddr(shortAddr.c_str(), &this->shortAddr);
        readGroupAddr(longAddr.c_str(), &this->longAddr);
    }
};

std::map<string, Device *> devices;

void callback(Plugin *context, string action, Channel *c, json raw) {
    if (action == "query") {
        for (pair<const string, Device *> &d : devices) {
            string deviceChannel = d.first;

            // Remove the values from the attributes
            vector<string> attributes;
            json attr = d.second->getAttributes();
            for (json::iterator it = attr.begin(); it != attr.end(); ++it) {
                attributes.push_back(it.key());
            }

            // Arrange the device description
            json dev = {
                    {"action",     "announce"},
                    {"type",       d.second->getDeviceType()},
                    {"name",       d.second->getDeviceName()},
                    {"channel",    Channel(deviceChannel).getAddress()},
                    {"attributes", attributes}
            };

            // Send it to the clients
            context->outgoingDatagrams.add(dev.dump());
        }
    } else if (action == "write" && c != nullptr && devices.find(c->getAddressAsString()) != devices.end()) {
        devices[c->getAddressAsString()]->set(raw["payload"]);
    }
};

int init(Plugin *context) {
    connectionURL = context->config["connection"];
    for (json device : context->config["devices"]) {
        Device *dev;

        if (device["type"] == "fixture") {
            dev = new Fixture(device["name"], device["attributes"]);
        } else if (device["type"] == "shutter") {
            dev = new Shutter(device["name"], device["attributes"]);
        }

        vector<int> channel = device["channel"];
        devices[Channel(channel[0], channel[1], channel[2]).getAddressAsString()] = dev;
    }

    return 0;
}

extern "C" Plugin* load_plugin() {
    return new Plugin(callback, init, "KNX", "0.0.2", true);
}

extern "C" void unload_plugin(Plugin* p) {
    delete p;
}