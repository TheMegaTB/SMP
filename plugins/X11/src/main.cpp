#include <Logger.hpp>
#include <thread>
#include "Plugin.hpp"
#include "xhklib.hpp"

using json = nlohmann::json;

// Call this when "H" is pressed.
void print_hello(xhkEvent e, void *r1, void *r2, void *r3) {
    printf("Hello\n");
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

void keyThread(Plugin *context, xhkConfig *hkconfig) {
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
        KeySym sym = XStringToKeysym("A");

        // Mod5Mask = Alt-gr (see https://github.com/TheMegaTB/.files/blob/master/home/themegatb/.Xmodmap#L24 )
        xhkBindKey(hkconfig, 0, sym, Mod5Mask, xhkKeyPress, &print_hello, 0, 0, 0);
    } else {
        err("Keyconfig is invalid (not an object)");
    }

    thread keys(keyThread, context, hkconfig);
    keys.detach(); // Let the thread continue to run in the background
    return 0;
}

extern "C" Plugin *load_plugin() {
    return new Plugin(callback, init, "X11", "0.0.2", true);
}

extern "C" void unload_plugin(Plugin *p) {
    delete p;
}