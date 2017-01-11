#include <iostream>
#include <thread>
#include <csignal>
#include <dlfcn.h>

#include "Plugin.hpp"
#include "PluginHandler.hpp"
#include "InterruptHandle.hpp"

#define RECV_TIMEOUT 2000

using namespace std;

void receiveData(PluginHandler *p, InterruptHandle *handle) {
    while (!(*handle).isInterrupted()) {
        p->receiveData(RECV_TIMEOUT);
    }
}

void processData(PluginHandler *p, InterruptHandle *handle) {
    while (!(*handle).isInterrupted()) {
        p->processData();
    }
}

volatile sig_atomic_t interrupted = 0;

void onInterrupt(int) {
    interrupted = 1;
}

int loadPlugin() {
    using std::cout;
    using std::cerr;

    // load the plugin library
    void* plugin = dlopen("/home/themegatb/Projects/C++/sh/cmake-build-debug/plugin_gpio/libgpio.so", RTLD_LAZY);
    if (!plugin) {
        cerr << "Cannot load library: " << dlerror() << '\n';
        return 1;
    }

    // reset errors
    dlerror();

    // load the symbols
    load_plugin_t* load_plugin = (load_plugin_t*) dlsym(plugin, "load_plugin");
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol create: " << dlsym_error << '\n';
        return 1;
    }

    unload_plugin_t* unload_plugin = (unload_plugin_t *) dlsym(plugin, "unload_plugin");
    dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol destroy: " << dlsym_error << '\n';
        return 1;
    }

    Plugin* plug = load_plugin();

    PluginHandler ph("224.0.0.1", 1234);
    ph.addPlugin(plug);

    InterruptHandle handle;
    thread networking(receiveData, &ph, &handle);
    thread processing(processData, &ph, &handle);

    cout << "Awaiting requests . . ." << endl;
    while (interrupted == 0)
        sleep(1);

    cout << "\nInterrupted! Exiting..." << endl;
    handle.interrupt();
    networking.join();
    processing.join();

    unload_plugin(plug);

    // unload the plugin library
    dlclose(plugin);
}

int main() {

    signal(SIGINT, onInterrupt);

    loadPlugin();

    return 0;
}