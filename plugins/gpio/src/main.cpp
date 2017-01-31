#include <Logger.hpp>
#include "Plugin.hpp"

using json = nlohmann::json;

void callback(Plugin *context, string action, Channel *c, json p) {
//    trace("WRITE");
//    trace(c->getAddressAsString());
//    trace(p.dump());
}

int init(Plugin *context) {
    return 0;
}

extern "C" Plugin* load_plugin() {
    return new Plugin(callback, init, "GPIO", "0.0.2", false);
}

extern "C" void unload_plugin(Plugin* p) {
    delete p;
}