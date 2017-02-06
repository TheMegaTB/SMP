#include <Logger.hpp>
#include "Plugin.hpp"

using json = nlohmann::json;

void callback(Plugin *context, string action, Channel *c, json p) {
    if (action == "query") {
        for (json::iterator it = context->config.begin(); it != context->config.end(); ++it) {

            // Arrange the device description
            json dev = {
                    {"action",     "announce"},
                    {"type",       "Scene"},
                    {"name",       it.value()["name"]},
                    {"channel",    Channel(it.key()).getAddress()},
                    {"attributes", {"binary"}}
            };

            // Send it to the clients
            context->outgoingDatagrams.add(dev.dump());
        }
    } else if (c != nullptr && context->config[c->getAddressAsString()].is_object()) {
        if (!(p["payload"].is_number() || p["payload"].is_boolean())) return;
        int value = p["payload"];

        json sequence;
        if ((bool) value) {
            sequence = context->config[c->getAddressAsString()]["on"];
        } else {
            sequence = context->config[c->getAddressAsString()]["off"];
        }

        for (json::iterator it = sequence.begin(); it != sequence.end(); ++it) {
            json payload = (*it)[3];
            (*it).erase((*it).begin() + 3);

            json dev = {
                    {"action",  "write"},
                    {"channel", (*it)},
                    {"payload", payload}
            };
            context->outgoingDatagrams.add(dev.dump());
        }
    }
}

int init(Plugin *context) {
    return 0;
}

extern "C" Plugin* load_plugin() {
    return new Plugin(callback, init, "Scenes", "0.0.1", true);
}

extern "C" void unload_plugin(Plugin* p) {
    delete p;
}