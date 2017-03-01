//
// Created by themegatb on 1/3/17.
//

#ifndef SMP_PLUGIN_HPP
#define SMP_PLUGIN_HPP

#include <string>
#include <fstream>
#include <Logger.hpp>
#include "Channel.hpp"
#include "json.hpp"
#include "EventQueue.hpp"
#include "UDPSocket.hpp"
#include "InterruptHandle.hpp"

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
    bool loadConfig;
    string version;

public:
    InterruptHandle interruptHandle;
    string name;
    json config;
    SafeQueue<string> outgoingDatagrams;

    string getDescriptor();
    void process(json datagram) override;

    template<class... Args>
    void createThread(Args &&... args) {
        this->interruptHandle.handles.emplace_back(std::forward<Args>(args)..., this);
    }

    int init();
    Plugin(pluginCallback, pluginInit, string name, string version, bool loadConfig);
};

typedef Plugin* load_plugin_t();
typedef void unload_plugin_t(Plugin*);


#endif //SMP_PLUGIN_HPP
