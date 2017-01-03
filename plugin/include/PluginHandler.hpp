//
// Created by themegatb on 1/3/17.
//

#ifndef SMARTHOME_PLUGINHANDLER_HPP
#define SMARTHOME_PLUGINHANDLER_HPP

#include <vector>
#include <chrono>
#include <string>
#include "Plugin.hpp"
#include "SafeQueue.hpp"
#include "EventQueue.hpp"
#include "UDPSocket.hpp"
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

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
