#include "../include/PluginHandler.hpp"

int PluginHandler::addPlugin(Plugin *plugin) {
//    plugin->setSocket(&this->sock);

    if ((*plugin).init() > 0) {
        err("Failed to initialize " + (*plugin).name + " plugin.");
        return 1;
    }

    this->plugins.addObserver(plugin);
    return 0;
}

int PluginHandler::sendData() {
    // Harvest strings from plugins and send them to the UDP network
    for (Observer<json> *plugin : this->plugins.observers) {
        string packet;
        Plugin *p = (Plugin *) plugin;
        while (p->outgoingDatagrams.take(&packet, std::chrono::seconds(0)) < 1) {
            sock.send(packet);
#ifdef LOOPBACK
            try {
                json j = json::parse(packet);
                this->datagrams.add(j);
            } catch (std::invalid_argument) {
                return 1;
            }
#endif
        }
    }

    return 0;
}

int PluginHandler::receiveData(unsigned int timeout_ms) { // TODO Bad function name since it sends packets as well
    // Receive data from UDP network
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