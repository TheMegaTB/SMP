#include <Logger.hpp>
#include "Plugin.hpp"

using json = nlohmann::json;

void callback(Plugin *context, string action, Channel *c, json p) {
    json mappings = context->config["mappings"];
    if (c != nullptr && mappings[c->getAddressAsString()].is_object()) {
        int value = p["payload"];

        json sequence;
        if ((bool) value) {
            sequence = mappings[c->getAddressAsString()]["on"];
        } else {
            sequence = mappings[c->getAddressAsString()]["off"];
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