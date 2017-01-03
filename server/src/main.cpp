#include <iostream>
#include <thread>
#include <csignal>
#include <dlfcn.h>

#include "json.hpp"
#include "Channel.hpp"
#include "PluginHandler.hpp"
#include "InterruptHandle.hpp"

#define RECV_TIMEOUT 2000

// for convenience
using json = nlohmann::json;
using namespace std;

// PLUGIN CODE
void wc(Channel c, json p) {
    cout << "WRITE" << endl;
    cout << c.getAddressAsString() << endl;
    cout << p.dump(4) << endl;
};

json rc(Channel c, json p) {
    cout << "READ" << endl;
    cout << c.getAddressAsString() << endl;
    cout << p.dump(4) << endl;
    json j;
    return j;
};
// PLUGIN CODE END

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

    cout << "C++ dlopen demo\n\n";

    // open the library
    cout << "Opening hello.so...\n";
    void *handle = dlopen("/home/themegatb/Projects/C++/smartHome/plugin_gpio/build/libgpio.so", RTLD_LAZY);

    if (!handle) {
        cerr << "Cannot open library: " << dlerror() << '\n';
        return 1;
    }

    // load the symbol
    cout << "Loading symbol hello...\n";
    typedef void (*hello_t)();

    // reset errors
    dlerror();
    hello_t hello = (hello_t) dlsym(handle, "hello");
    const char *dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol 'hello': " << dlsym_error <<
             '\n';
        dlclose(handle);
        return 1;
    }

    // use it to do the calculation
    cout << "Calling hello...\n";
    hello();

    // close the library
    cout << "Closing library...\n";
    dlclose(handle);

    return 0;
}

int main() {

    loadPlugin();

    signal(SIGINT, onInterrupt);

    PluginHandler ph("224.0.0.1", 1234);

    Plugin p = Plugin(&rc, &wc);
    ph.addPlugin(&p);


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

    return 0;
}