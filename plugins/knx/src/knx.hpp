//
// Created by themegatb on 1/24/17.
//

#ifndef SMARTHOME_KNX_HPP
#define SMARTHOME_KNX_HPP

#include <eibclient.h>
#include <Logger.hpp>

#define DEFAULT_CONN_URL "ip:localhost"

extern string connectionURL;

EIBConnection *connectEIB();

int readGroupAddr(const char *addr, eibaddr_t *groupAddr);

int sendDatagram(eibaddr_t dest, unsigned char *data, int len);

int dimLight(string address, unsigned char value);

int switchLight(string address, bool val);


#endif //SMARTHOME_KNX_HPP
