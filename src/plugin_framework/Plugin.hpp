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

typedef json (*readCallback)(Channel, json payload);

typedef void (*writeCallback)(Channel, json payload);
//TODO: Implement filtering of requests to reduce load
// Hardware layer interface
class Plugin {
public:
    void processJSON(json datagram);

    Plugin(readCallback r, writeCallback w) {
        this->readCB = r;
        this->writeCB = w;
    }

private:
    bool state;
    readCallback readCB; // TODO: Make this optional
    writeCallback writeCB;
};


#endif //SMARTHOME_PLUGIN_HPP
