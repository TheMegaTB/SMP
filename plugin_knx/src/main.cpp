#include <Logger.hpp>
#include "Plugin.hpp"

using json = nlohmann::json;

void wc(Channel c, json p) {
    trace("WRITE");
};

json rc(Channel c, json p) {
    trace("READ");
    json j;
    return j.dump();
};

extern "C" Plugin* load_plugin() {
    return new Plugin(rc, wc, "KNX", "0.0.1");
}

extern "C" void unload_plugin(Plugin* p) {
    delete p;
}