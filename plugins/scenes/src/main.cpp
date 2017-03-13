#include <Logger.hpp>
#include "Plugin.hpp"

using json = nlohmann::json;

typedef std::map<std::string, std::string> Scene;

std::map<std::string, Scene> activeScenes;

void callback(Plugin *context, string action, Channel *c, json p) {
    bool sceneExists = (c != nullptr && context->config[c->getAddressAsString()].is_object());
    if (action == "query") {
        for (json::iterator it = context->config.begin(); it != context->config.end(); ++it) {
            if (!it.value().is_object()) continue;

            // Arrange the device description
            json dev = {
                    {"action",     "announce"},
                    {"type",       "Scene"},
                    {"name",       it.value()["name"]},
                    {"channel",    Channel(it.key()).getAddress()},
                    {"attributes", {"binary"}}
            };

            // Send it to the clients
            context->outgoingDatagrams.add(dev.dump());
        }
    } else if (action == "write" && sceneExists) {
        if (!(p["payload"].is_number() || p["payload"].is_boolean())) return;
        int value = p["payload"];

        json sequence;
        Scene newScene;
        if ((bool) value) {
            sequence = context->config[c->getAddressAsString()]["on"];
        } else {
            sequence = context->config[c->getAddressAsString()]["off"];
            activeScenes.erase(c->getAddressAsString());
        }

        for (json::iterator it = sequence.begin(); it != sequence.end(); ++it) {
            json payload = (*it)[3];
            (*it).erase((*it).begin() + 3);

            vector<int> address = (*it);
            if ((bool) value) newScene[Channel(&address).getAddressAsString()] = payload.dump();

            json dev = {
                    {"action",    "write"},
                    {"automatic", true},
                    {"channel",   (*it)},
                    {"payload",   payload}
            };
            context->outgoingDatagrams.add(dev.dump());
        }

        if ((bool) value) activeScenes[c->getAddressAsString()] = newScene;

        json stateUpdate = {
                {"action",  "state"},
                {"channel", c->getAddress()},
                {"payload", (bool) value}
        };
        context->outgoingDatagrams.add(stateUpdate.dump());
    } else if (action == "read" && sceneExists) {
        json res = {
                {"action",  "state"},
                {"channel", c->getAddress()},
                {"payload", activeScenes.find(c->getAddressAsString()) != activeScenes.end()}
        };
        context->outgoingDatagrams.add(res.dump());
    } else if (action == "state") {
        vector<string> pendingDeletions;
        for (auto const &scene : activeScenes) {
            string currentSceneAddress = scene.first;
            Scene currentScene = scene.second;

            auto iter = currentScene.find(c->getAddressAsString());
            if (iter != currentScene.end() && p["payload"].dump() != iter->second) {
                // One device got changed so disable the scene!
                pendingDeletions.push_back(currentSceneAddress);
                Channel x(currentSceneAddress);

                json stateUpdate = {
                        {"action",  "state"},
                        {"channel", x.getAddress()},
                        {"payload", false}
                };
                context->outgoingDatagrams.add(stateUpdate.dump());
            }
        }
        for (string key : pendingDeletions) activeScenes.erase(key);
    }
}

int init(Plugin *context) {
    return 0;
}

extern "C" Plugin* load_plugin() {
    return new Plugin(callback, init, "Scenes", "0.0.1", true);
}

extern "C" void unload_plugin(Plugin* p) {
    delete p;
}