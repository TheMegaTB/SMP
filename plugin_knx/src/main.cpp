#include <Logger.hpp>
#include <eibclient.h>
#include "Plugin.hpp"
#include "common.hpp"

using json = nlohmann::json;

int turnLightOn() {
    debug("Turning light on");
    int len;
    EIBConnection *con;
    eibaddr_t dest;
    unsigned char buf[3] = {0, 0x80};

    const char *url = "ip:bedroom-node:4433";
    const char *groupAddr = "0/4/0";
    const char *value = "0x01";

    con = EIBSocketURL(url);
    if (!con) {
        error("Couldn't open connection to knxd");
        return 1;
    }

    dest = readgaddr(groupAddr);
    buf[1] |= readHex(value) & 0x3f;

    if (EIBOpenT_Group(con, dest, 1) == -1) {
        error("Failed to connect to device");
        return 1;
    }

    len = EIBSendAPDU(con, 2, buf);
    if (len == -1) {
        error("Request to device failed");
        return 1;
    }

    trace("Request sent");

    EIBClose(con);
    return 0;
}

int main() {
    turnLightOn();
}

void wc(Channel c, json p) {
    trace("WRITE");
    turnLightOn();
};

json rc(Channel c, json p) {
    trace("READ");
    json j;
    return j.dump();
};

extern "C" Plugin* load_plugin() {
    return new Plugin(rc, wc, "KNX", "0.0.1");
}

extern "C" void unload_plugin(Plugin* p) {
    delete p;
}