#include "../include/PluginHandler.hpp"

void PluginHandler::addPlugin(Plugin *plugin) {
    this->plugins.addObserver(plugin);
}

int PluginHandler::receiveData(unsigned int timeout_ms) {
    std::string data;
    int res = sock.recv(&data, timeout_ms);
    if (res > 0)
        this->datagrams.add(data);
    return res;
}

int PluginHandler::processData() {
    int processed = 0;
    while (this->plugins.observeOnce(std::chrono::milliseconds(200)) > 0)
        ++processed;
    return processed;
}

PluginHandler::PluginHandler(string multicastAddr, uint16_t port)
        : sock(multicastAddr, port), datagrams(), plugins(&datagrams) {}