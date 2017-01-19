//
// Created by themegatb on 1/19/17.
//

#ifndef SMARTHOME_PLUGINLOADER_H
#define SMARTHOME_PLUGINLOADER_H

#include <dlfcn.h>

#include <Plugin.hpp>
#include <PluginHandler.hpp>

using namespace std;

typedef tuple<Plugin *, unload_plugin_t *, void *> pluginHook;

class PluginLoader {
    vector<pluginHook> plugins;
    string pluginDir;

public:
    PluginHandler pluginHandler;

    void setPluginDir(string path);

    int loadPlugin(string name);

    PluginLoader(string multicastAddr, uint16_t port);

    ~PluginLoader();
};


#endif //SMARTHOME_PLUGINLOADER_H
