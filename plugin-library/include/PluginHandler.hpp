//
// Created by themegatb on 1/3/17.
//

#ifndef SMARTHOME_PLUGINHANDLER_HPP
#define SMARTHOME_PLUGINHANDLER_HPP

#include <vector>
#include <chrono>
#include <string>
#include "json.hpp"
#include "Plugin.hpp"
#include "SafeQueue.hpp"
#include "EventQueue.hpp"
#include "UDPSocket.hpp"

using namespace std;
using json = nlohmann::json;

class PluginHandler {
    SafeQueue<json> datagrams;
    EventQueue<json> plugins;
    UDPSocket sock;
public:
    void addPlugin(Plugin *plugin);

    int receiveData(unsigned int timeout_ms);

    int processData();

    PluginHandler(string multicastAddr, uint16_t port);
};


#endif //SMARTHOME_PLUGINHANDLER_HPP
