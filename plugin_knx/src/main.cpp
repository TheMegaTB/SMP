#include <Logger.hpp>
#include <eibclient.h>
#include "Plugin.hpp"

using json = nlohmann::json;

#define SOCKET_URL "ip:bedroom-node:4433"

EIBConnection *connectEIB() {
    EIBConnection *con = EIBSocketURL(SOCKET_URL);
    if (!con) {
        error("Couldn't open connection to knxd");
        return nullptr;
    }

    return con;
}

int readGroupAddr(const char *addr, eibaddr_t *groupAddr) {
    unsigned int a, b, c, res;
    res = (unsigned int) sscanf(addr, "%u/%u/%u", &a, &b, &c);
    if (res == 3 && a <= 0x1F && b <= 0x07 && c <= 0xFF) {
        *groupAddr = (eibaddr_t) ((a << 11) | (b << 8) | c);
        return 0;
    }
    if (res == 2 && a <= 0x1F && b <= 0x7FF) {
        *groupAddr = (eibaddr_t) ((a << 11) | (b & 0x7FF));
        return 0;
    }
    if (sscanf(addr, "%x", &a) == 1 && a <= 0xFFFF) {
        *groupAddr = (eibaddr_t) a;
        return 0;
    }
    error("Invalid group address format");
    return 1;
}

int sendDatagram(eibaddr_t dest, unsigned char *data, int len) {
    EIBConnection *con = connectEIB();

    if (con == nullptr) {
        return 1;
    }


    if (EIBOpenT_Group(con, dest, 1) == -1) {
        error("Failed to connect to device");
        return 1;
    }

    if (EIBSendAPDU(con, len, data) == -1) {
        error("Request to device failed");
        return 1;
    }

    EIBClose(con);
    return 0;
}

int dimLight(string address, unsigned char value) {
    eibaddr_t dest;
    readGroupAddr(address.c_str(), &dest);

    unsigned char buf[255] = {0, 0x80, value};

    return sendDatagram(dest, buf, 3);
}

int switchLight(string address, bool val) {
    eibaddr_t dest;
    readGroupAddr(address.c_str(), &dest);
    unsigned char buf[3] = {0, 0x80};
    buf[1] |= val;

    return sendDatagram(dest, buf, 2);
}

void callback(string action, Channel c, json p) {
    trace(action);
    trace(c.getAddressAsString());
    trace(p.dump());

//    switchLight("0/4/0", false);
    dimLight("0/4/2", 20);
};

extern "C" Plugin* load_plugin() {
    return new Plugin(callback, "KNX", "0.0.1");
}

extern "C" void unload_plugin(Plugin* p) {
    delete p;
}