//
// Created by themegatb on 1/3/17.
//

#ifndef SMARTHOME_PLUGIN_HPP
#define SMARTHOME_PLUGIN_HPP

#include <string>
#include <Logger.hpp>
#include "Channel.hpp"
#include "json.hpp"
#include "EventQueue.hpp"

using json = nlohmann::json;
using namespace std;

//typedef json (*readCallback)(Channel, json payload);
//typedef void (*writeCallback)(Channel, json payload);

typedef void (*pluginCallback)(string action, Channel, json payload);

typedef int (*pluginInit)();

//TODO: Implement filtering of requests to reduce load
// Hardware layer interface
class Plugin : public Observer<json> {
    pluginCallback callback;
    pluginInit initFunc;
    bool initialized;

    string version;

public:
    string getDescriptor();
    void process(json datagram) override;

    int init();

    Plugin(pluginCallback, pluginInit, string name, string version);

    string name;
};

typedef Plugin* load_plugin_t();
typedef void unload_plugin_t(Plugin*);


#endif //SMARTHOME_PLUGIN_HPP
