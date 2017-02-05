#include <Logger.hpp>
#include "Plugin.hpp"

using json = nlohmann::json;

void callback(Plugin *context, string action, Channel *c, json p) {

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