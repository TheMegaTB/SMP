//
// Created by themegatb on 1/3/17.
//

#ifndef SMARTHOME_PLUGIN_HPP
#define SMARTHOME_PLUGIN_HPP

#include <string>
#include "Channel.hpp"
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

//TODO: Implement filtering of requests to reduce load
// Hardware layer interface
class Plugin {
public:
    typedef json (Plugin::* readCallback)(Channel);

    typedef void (Plugin::* writeCallback)(Channel, json payload);

    void processJSON(json datagram);

    Plugin(Plugin::readCallback r, Plugin::writeCallback w) {
        this->readCB = r;
        this->writeCB = w;
    }

private:
    bool state;
    Plugin::readCallback readCB; // TODO: Make this optional
    Plugin::writeCallback writeCB;
};


#endif //SMARTHOME_PLUGIN_HPP
