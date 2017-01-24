#include <Logger.hpp>
#include "Plugin.hpp"

using json = nlohmann::json;

void callback(string action, Channel c, json p) {
    trace("WRITE");
    trace(c.getAddressAsString());
    trace(p.dump());
}

int init() {
    return 0;
}

extern "C" Plugin *load_plugin() {
    return new Plugin(callback, init, "REST", "0.0.1");
}

extern "C" void unload_plugin(Plugin *p) {
    delete p;
}