#include <Logger.hpp>
#include <thread>
#include "Plugin.hpp"
#include "xhklib.hpp"

using json = nlohmann::json;

std::map<KeySym, vector<int>> mappings;

void handleKeypress(xhkEvent e, void *context, void *r2, void *r3) {
    vector<int> target = mappings[e.keysym];
    int payload = target[3];
    target.erase(target.begin() + 3);

    Plugin *c = (Plugin *) context;
    json dev = {
            {"action",  "write"},
            {"channel", target},
            {"payload", payload}
    };
    c->outgoingDatagrams.add(dev.dump());
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

void keyThread(xhkConfig *hkconfig, Plugin *context) {
    while (1) {
        xhkPollKeys(hkconfig, 1);
    }

    xhkClose(hkconfig);
}

#pragma clang diagnostic pop

void callback(Plugin *context, string action, Channel *c, json p) {
    return;
}

int init(Plugin *context) {
    xhkConfig *hkconfig;
    hkconfig = xhkInit(NULL);

    if (context->config["keys"].is_object()) {
        json keys = context->config["keys"];
        unsigned int modifier = Mod1Mask;   // = Alt-gr (see https://github.com/TheMegaTB/.files/blob/master/home/themegatb/.Xmodmap#L24 )

        for (json::iterator it = keys.begin(); it != keys.end(); ++it) {
            json j = it.value();
            if (!j.is_array()) {
                err("Keyconfig is invalid (property of keycode is not an array)");
                return 1;
            }

            std::vector<int> target = j;

            KeySym sym = XStringToKeysym(string(it.key()).c_str());
            mappings[sym] = target;

            xhkBindKey(hkconfig, 0, sym, modifier, xhkKeyPress, &handleKeypress, context, 0, 0);
        }
    } else {
        err("Keyconfig is invalid (not an object)");
        return 1;
    }

    context->createThread(keyThread, hkconfig);
    return 0;
}

extern "C" Plugin *load_plugin() {
    return new Plugin(callback, init, "X11", "0.0.2", true);
}

extern "C" void unload_plugin(Plugin *p) {
    delete p;
}
