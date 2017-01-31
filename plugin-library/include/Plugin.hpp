//
// Created by themegatb on 1/3/17.
//

#ifndef SMP_PLUGIN_HPP
#define SMP_PLUGIN_HPP

#include <string>
#include <Logger.hpp>
#include "Channel.hpp"
#include "json.hpp"
#include "EventQueue.hpp"
#include "UDPSocket.hpp"

using json = nlohmann::json;
using namespace std;

class Plugin;

typedef void (*pluginCallback)(Plugin *, string action, Channel *, json rawData);

typedef int (*pluginInit)(Plugin *);

// Hardware layer interface
class Plugin : public Observer<json> {
    pluginCallback callback;
    pluginInit initFunc;
    bool initialized;
    string version;

public:
    string name;
    SafeQueue<string> outgoingDatagrams;

    string getDescriptor();
    void process(json datagram) override;

//    void setSocket(UDPSocket *);

    int init();
    Plugin(pluginCallback, pluginInit, string name, string version);
};

typedef Plugin* load_plugin_t();
typedef void unload_plugin_t(Plugin*);


#endif //SMP_PLUGIN_HPP
