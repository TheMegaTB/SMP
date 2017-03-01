//
// Created by themegatb on 1/24/17.
//

#ifndef SMP_KNX_HPP
#define SMP_KNX_HPP

#include <eibclient.h>
#include <Logger.hpp>
#include <Plugin.hpp>

#define DEFAULT_CONN_URL "ip:localhost"

extern string connectionURL;

EIBConnection *connectEIB();

int readGroupAddr(const char *addr, eibaddr_t *groupAddr);

int sendDatagram(eibaddr_t dest, unsigned char *data, int len);

int dimLight(eibaddr_t address, unsigned char value);

int switchLight(eibaddr_t address, bool val);

void readPackages(eibaddr_t dest, Plugin *context);


#endif //SMP_KNX_HPP
