#include <string>
#include <thread>

#include "Plugin.hpp"
#include "knx.hpp"

#define SHIFT_STEP 10

using json = nlohmann::json;

class Device {
public:
    virtual void set(json, bool updateHardware = true)= 0;
    virtual json getAttributes()= 0;
    virtual const char* getDeviceType() { return "Device"; }
    virtual string getDeviceName() { return "Unnamed device"; }

    virtual void sendUpdate()= 0;
};

class Fixture : public Device {
    eibaddr_t binaryAddr;
    eibaddr_t dimmableAddr;
    eibaddr_t shiftableAddr;
    bool dimmable;

    int currentValue = 0;
    Channel channel;
    Plugin *context;

    json attributes;
    string name;

public:

    const char* getDeviceType() override {
        return "Fixture";
    }

    string getDeviceName() override {
        return this->name;
    }

    void set(json value, bool updateHardware = true) override {
        if (value.is_boolean()) {
            switchLight(this->binaryAddr, value);
            this->currentValue = value ? 255 : 0;
        } else if (value.is_number_integer() && this->dimmable) {
            dimLight(this->dimmableAddr, value);
            this->currentValue = value;
        }
//        this->sendUpdate(); // Rely on the feedback from the KNX network
    }

    json getAttributes() override {
        return this->attributes;
    }

    void sendUpdate() {
        json stateUpdate = {
                {"action",  "state"},
                {"channel", this->channel.getAddress()},
        };
        if (this->dimmable)
            stateUpdate["payload"] = this->currentValue;
        else
            stateUpdate["payload"] = this->currentValue != 0;

        this->context->outgoingDatagrams.add(stateUpdate.dump());
    }

    void binaryListener() {
        EIBConnection *con = connectEIB();
        unsigned char buf[255];
        int len;
        eibaddr_t src;

        if (EIBOpenT_Group(con, this->binaryAddr, 0) == -1) err("Connect failed");

        while (!context->interruptHandle.isInterrupted()) {
            len = EIBGetAPDU_Src(con, sizeof(buf), buf, &src);
            if (len == -1) err("Read failed");
            if (len < 2) err("Invalid Packet");
            if (buf[0] & 0x3 || (buf[1] & 0xC0) == 0xC0) {
                err("Unknown APDU.");
            } else if ((buf[1] & 0xC0) == 0x80 && len == 2) { // If type is "write"
                this->currentValue = (buf[1] & 0x3F) ? 255 : 0;
                this->sendUpdate();
            }
        }
    }

    void valueListener() {
        EIBConnection *con = connectEIB();
        unsigned char buf[255];
        int len;
        eibaddr_t src;

        if (EIBOpenT_Group(con, this->dimmableAddr, 0) == -1) err("Connect failed");

        while (!context->interruptHandle.isInterrupted()) {
            len = EIBGetAPDU_Src(con, sizeof(buf), buf, &src);
            if (len == -1) err("Read failed");
            if (len < 2) err("Invalid Packet");
            if (buf[0] & 0x3 || (buf[1] & 0xC0) == 0xC0) {
                err("Unknown APDU.");
            } else if ((buf[1] & 0xC0) == 0x80) { // If type is "write"
                if (len == 2) {
                    this->currentValue = (buf[1] & 0x3F) ? 255 : 0;
                    this->sendUpdate();
                } else {
                    // TODO Support more than 1-Byte values
                    if (len - 2 == 1) {
                        this->currentValue = (buf + 2)[0];
                        this->sendUpdate();
                    }
                }
            }
        }
    }

    void shiftListener() {
        EIBConnection *con = connectEIB();
        unsigned char buf[255];
        int len;
        eibaddr_t src;

        if (EIBOpenT_Group(con, this->shiftableAddr, 0) == -1) err("Connect failed");

        while (!context->interruptHandle.isInterrupted()) {
            len = EIBGetAPDU_Src(con, sizeof(buf), buf, &src);
            if (len == -1) err("Read failed");
            if (len < 2) err("Invalid Packet");
            if (buf[0] & 0x3 || (buf[1] & 0xC0) == 0xC0) {
                err("Unknown APDU.");
            } else if ((buf[1] & 0xC0) == 0x80) { // If type is "write"
                if (len == 2) {
                    int action = buf[1] & 0x3F;
                    int val = this->currentValue;
                    if (action == 12 && this->currentValue <= (255 - SHIFT_STEP))
                        this->currentValue += SHIFT_STEP;
                    else if (action == 4 && this->currentValue > SHIFT_STEP)
                        this->currentValue -= SHIFT_STEP;
                    sendUpdate();
                }
            }
        }
    }

    Fixture(json name, json attributes, Channel c, Plugin *cont) : channel(c), context(cont) {
        this->attributes = attributes;
        this->name = name;

        string binaryAddr = attributes["binary"];
        readGroupAddr(binaryAddr.c_str(), &this->binaryAddr);
        context->createThread(std::bind(&Fixture::binaryListener, this));

        if (attributes["dimmable"].is_string()) {
            this->dimmable = true;
            string dimmableAddr = attributes["dimmable"];
            readGroupAddr(dimmableAddr.c_str(), &this->dimmableAddr);
            context->createThread(std::bind(&Fixture::valueListener, this));
        }

        if (attributes["shiftable"].is_string()) {
            string shiftableAddr = attributes["shiftable"];
            readGroupAddr(shiftableAddr.c_str(), &this->shiftableAddr);
            context->createThread(std::bind(&Fixture::shiftListener, this));
        }
    }
};

class Shutter : public Device {
    eibaddr_t shortAddr;
    eibaddr_t longAddr;

    json attributes;
    string name;

    string currentValue;
    Channel channel;
    Plugin *context;

public:

    const char* getDeviceType() override {
        return "Shutter";
    }

    string getDeviceName() override {
        return this->name;
    }

    void set(json value, bool updateHardware = true) override {
        // TODO Implement some logic for 3/4 up and 3/4 down
        if (value.is_string()) {
            if (value == "up")
                switchLight(longAddr, 0);
            else if (value == "down")
                switchLight(longAddr, 1);
            else if (value == "stop")
                switchLight(shortAddr, 0);
            else return;

            this->currentValue = value;

            this->sendUpdate();
        } else if (value.is_boolean()) {
            switchLight(longAddr, value);
        }
    }

    void sendUpdate() {
        json stateUpdate = {
                {"action",  "state"},
                {"channel", this->channel.getAddress()},
                {"payload", this->currentValue}
        };

        this->context->outgoingDatagrams.add(stateUpdate.dump());
    }

    json getAttributes() override {
        json tmp = this->attributes;
        tmp["binary"] = "";
        return tmp;
    }

    Shutter(json name, json attributes, Channel c, Plugin *cont) : channel(c), context(cont) {
        this->attributes = attributes;
        this->name = name;

        string shortAddr = attributes["short"];
        string longAddr = attributes["long"];
        readGroupAddr(shortAddr.c_str(), &this->shortAddr);
        readGroupAddr(longAddr.c_str(), &this->longAddr);

        // TODO Add listener that updates the state
    }
};

std::map<string, Device *> devices;

void callback(Plugin *context, string action, Channel *c, json raw) {
    bool deviceExists = (c != nullptr && devices.find(c->getAddressAsString()) != devices.end());

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
    } else if (action == "write" && deviceExists) {
        devices[c->getAddressAsString()]->set(raw["payload"]);
    } else if (action == "read" && deviceExists) {
        devices[c->getAddressAsString()]->sendUpdate();
    }
};

int init(Plugin *context) {
    // Populate devices
    connectionURL = context->config["connection"];
    for (json device : context->config["devices"]) {

        Channel channel = Channel(device["channel"][0], device["channel"][1], device["channel"][2]);
        Device *dev;

        if (device["type"] == "fixture") {
            dev = new Fixture(device["name"], device["attributes"], channel, context);
        } else if (device["type"] == "shutter") {
            dev = new Shutter(device["name"], device["attributes"], channel, context);
        } else {
            continue;
        }

        // Add device to list
        devices[channel.getAddressAsString()] = dev;
    }

    return 0;
}

extern "C" Plugin* load_plugin() {
    return new Plugin(callback, init, "KNX", "0.0.2", true);
}

extern "C" void unload_plugin(Plugin* p) {
    delete p;
}