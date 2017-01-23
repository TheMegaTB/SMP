#include <Logger.hpp>
#include <eibclient.h>
#include "Plugin.hpp"
#include "common.hpp"

using json = nlohmann::json;

#define SOCKET_URL "ip:bedroom-node:4433"

int dimLight(string address, int value) {
    int len;
    EIBConnection *con;
    eibaddr_t dest;
    uchar buf[255] = {0, 0x80};

    con = EIBSocketURL(SOCKET_URL);
    if (!con) {
        error("Couldn't open connection to knxd");
        return 1;
    }

    char *val = {(char *) "0x0F"};

    dest = readgaddr(address.c_str());
    len = readBlock(buf + 2, sizeof(buf) - 2, 1, &val);
    trace(to_string(len));

    if (EIBOpenT_Group(con, dest, 1) == -1) {
        error("Failed to connect to device");
        return 1;
    }

    len = EIBSendAPDU(con, 2 + len, buf);
    if (len == -1) {
        error("Request to device failed");
        return 1;
    }

    EIBClose(con);
    return 0;
}

int switchLight(string address, bool val) {
    int len;
    EIBConnection *con;
    eibaddr_t dest;
    unsigned char buf[3] = {0, 0x80};

    const char *value = val ? "0x01" : "0x00";

    con = EIBSocketURL(SOCKET_URL);
    if (!con) {
        error("Couldn't open connection to knxd");
        return 1;
    }

    dest = readgaddr(address.c_str());
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

    EIBClose(con);
    return 0;
}

void wc(Channel c, json p) {
    trace("WRITE");
//    switchLight("0/4/0", false);
    dimLight("0/4/2", 255);
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