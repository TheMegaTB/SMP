//
// Created by themegatb on 1/19/17.
//

#ifndef SMP_PLUGINLOADER_H
#define SMP_PLUGINLOADER_H

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

    void interruptAll();

    void waitForAll();

    PluginLoader(string multicastAddr, uint16_t port);

    ~PluginLoader();
};


#endif //SMP_PLUGINLOADER_H
