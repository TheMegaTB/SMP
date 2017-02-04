//
// Created by themegatb on 1/24/17.
//

#include "knx.hpp"

string connectionURL = DEFAULT_CONN_URL;

EIBConnection *connectEIB() {
    EIBConnection *con = EIBSocketURL(connectionURL.c_str());
    if (!con) {
        err("Couldn't open connection to knxd");
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
    err("Invalid group address format");
    return 1;
}

int sendDatagram(eibaddr_t dest, unsigned char *data, int len) {
    EIBConnection *con = connectEIB();

    if (con == nullptr) {
        return 1;
    }


    if (EIBOpenT_Group(con, dest, 1) == -1) {
        err("Failed to connect to device");
        return 1;
    }

    if (EIBSendAPDU(con, len, data) == -1) {
        err("Request to device failed");
        return 1;
    }

    EIBClose(con);
    return 0;
}

int dimLight(eibaddr_t address, unsigned char value) {
    unsigned char buf[255] = {0, 0x80, value};

    return sendDatagram(address, buf, 3);
}

int switchLight(eibaddr_t address, bool val) {
    unsigned char buf[3] = {0, 0x80};
    buf[1] |= val;

    return sendDatagram(address, buf, 2);
}

int readDatagrams(eibaddr_t address) {
    unsigned char buf[255];
    int len;
    EIBConnection *con = connectEIB();

    if (EIBOpenVBusmonitorText(con) == -1) {
        err("Open Busmonitor failed");
        return 1;
    }

    while (1) {
        len = EIBGetBusmonitorPacket(con, sizeof(buf), buf);
        if (len == -1) {
            err("Read failed");
            return 1;
        }
        buf[len] = 0;
//        printf ("%s\n", buf);
//        fflush (stdout);
    }

    EIBClose(con);
}