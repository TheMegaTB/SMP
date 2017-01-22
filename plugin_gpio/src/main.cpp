#include <Logger.hpp>
#include "Plugin.hpp"

using json = nlohmann::json;

void wc(Channel c, json p) {
    trace("WRITE");
    trace(c.getAddressAsString());
    trace(p.dump());
};

json rc(Channel c, json p) {
    trace("READ");
    trace(c.getAddressAsString());
    trace(p.dump());
    json j;
    return j.dump();
};

extern "C" Plugin* load_plugin() {
    return new Plugin(rc, wc, "GPIO", "0.0.1");
}

extern "C" void unload_plugin(Plugin* p) {
    delete p;
}