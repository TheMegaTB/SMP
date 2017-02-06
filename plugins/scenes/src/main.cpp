#include <Logger.hpp>
#include "Plugin.hpp"

using json = nlohmann::json;

void callback(Plugin *context, string action, Channel *c, json p) {
    if (c != nullptr && context->config[c->getAddressAsString()].is_object()) {
        debug("Received smth");
        trace(p.dump());
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