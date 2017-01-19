#include "Plugin.hpp"

using json = nlohmann::json;

void wc(Channel c, json p) {
    cout << "WRITE" << endl;
//    cout << c.getAddressAsString() << endl;
//    cout << p.dump(4) << endl;
};

json rc(Channel c, json p) {
    cout << "READ" << endl;
//    cout << c.getAddressAsString() << endl;
//    cout << p.dump(4) << endl;
//    cout << p["something"] << endl;
    json j;
    return j.dump(4);
};

extern "C" Plugin* load_plugin() {
    return new Plugin(rc, wc);
}

extern "C" void unload_plugin(Plugin* p) {
    delete p;
}