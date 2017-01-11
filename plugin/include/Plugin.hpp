//
// Created by themegatb on 1/3/17.
//

#ifndef SMARTHOME_PLUGIN_HPP
#define SMARTHOME_PLUGIN_HPP

#include <string>
#include "Channel.hpp"
#include "json.hpp"
#include "EventQueue.hpp"

using json = nlohmann::json;
using namespace std;

typedef json (*readCallback)(Channel, json payload);

typedef void (*writeCallback)(Channel, json payload);

//TODO: Implement filtering of requests to reduce load
// Hardware layer interface
class Plugin : public Observer<json> {
    bool state;
    readCallback readCB; // TODO: Make this optional
    writeCallback writeCB;

public:
    void process(json datagram) override;

    Plugin(readCallback r, writeCallback w);
};

typedef Plugin* load_plugin_t();
typedef void unload_plugin_t(Plugin*);


#endif //SMARTHOME_PLUGIN_HPP
