#include "../include/PluginHandler.hpp"

int PluginHandler::addPlugin(Plugin *plugin) {
    plugin->setSocket(&this->sock);

    if ((*plugin).init() > 0) {
        error("Failed to initialize " + (*plugin).name + " plugin.");
        return 1;
    }

    this->plugins.addObserver(plugin);
    return 0;
}

int PluginHandler::receiveData(unsigned int timeout_ms) {
    std::string data;
    int res = sock.recv(&data, timeout_ms);
    if (data.c_str() == NULL) return 1;
    if (res < 1) {
        json j;
        try {
            j = json::parse(data);
        } catch (std::invalid_argument) {
            return 1;
        }
        this->datagrams.add(j);
    }
    return res;
}

int PluginHandler::processData() {
    int processed = 0;
    while (this->plugins.observeOnce(std::chrono::milliseconds(200)) < 1)
        ++processed;
    return processed;
}

PluginHandler::PluginHandler(string multicastAddr, uint16_t port)
        : sock(multicastAddr, port), datagrams(), plugins(&datagrams) {}