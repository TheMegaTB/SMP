#include "Plugin.hpp"

#include "json.hpp"
#include <string>

using json = nlohmann::json;

void wc(int* c, string p) {
    cout << "WRITE" << endl;
    cout << c << endl;
    cout << p << endl;
};

std::string rc(int* c, string p) {
    cout << "READ" << endl;
    cout << c[0] << c[1] << c[2] << endl;
    cout << p << endl;
    json j;
    return j.dump(4);
};

extern "C" Plugin* load_plugin() {
    return new Plugin(rc, wc);
}

extern "C" void unload_plugin(Plugin* p) {
    delete p;
}