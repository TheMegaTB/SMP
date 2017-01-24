#include <string>
#include <fstream>

#include <Logger.hpp>
#include <stdlib.h>
#include "Plugin.hpp"
#include "knx.hpp"

using json = nlohmann::json;

int readConfigFile() {
    char *CONFIG_DIR = getenv("CONFIG_DIR");
    string confDir;
    if (CONFIG_DIR == NULL) {
        confDir = ".";
    } else {
        confDir = CONFIG_DIR;
    }

    std::ifstream t(confDir + "/knx.json");

    if (!t.good()) {
        error("KNX config file not found!");
        return 1;
    }

    json j;

    try {
        t >> j;
    } catch (const std::invalid_argument err) {
        error("Failed to parse KNX config file");
        return 1;
    }


    // TODO Use config

    debug(j.dump());

    return 0;
}

void callback(string action, Channel c, json p) {
    trace(action);
    trace(c.getAddressAsString());
    trace(p.dump());

//    switchLight("0/4/0", false);
    dimLight("0/4/2", 20);
};

int init() {
    return readConfigFile();
}

extern "C" Plugin* load_plugin() {
    return new Plugin(callback, init, "KNX", "0.0.1");
}

extern "C" void unload_plugin(Plugin* p) {
    delete p;
}