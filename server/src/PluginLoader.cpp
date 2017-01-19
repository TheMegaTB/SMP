//
// Created by themegatb on 1/19/17.
//

#include "PluginLoader.hpp"

PluginLoader::~PluginLoader() {
    // Unload all plugins and close their low level library handle
    for (pluginHook plugin : this->plugins) {
        get<1>(plugin)(get<0>(plugin));
        dlclose(get<2>(plugin));
    }
}

PluginLoader::PluginLoader(string multicastAddr, uint16_t port) : pluginHandler(multicastAddr, port) {}

int PluginLoader::loadPlugin(string name) {
    // load the plugin library
    void *pluginLib = dlopen((this->pluginDir + "/" + name).c_str(), RTLD_LAZY);
    if (!pluginLib) {
        cerr << "Cannot load library: " << dlerror() << '\n';
        return 1;
    }

    // reset errors
    dlerror();

    // load the symbols
    load_plugin_t *load_plugin = (load_plugin_t *) dlsym(pluginLib, "load_plugin");
    const char *dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol create: " << dlsym_error << '\n';
        return 1;
    }

    unload_plugin_t *unload_plugin = (unload_plugin_t *) dlsym(pluginLib, "unload_plugin");
    dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol destroy: " << dlsym_error << '\n';
        return 1;
    }

    Plugin *plugin = load_plugin();
    pluginHook ph = make_tuple(plugin, unload_plugin, pluginLib);
    this->pluginHandler.addPlugin(plugin);
    this->plugins.push_back(ph);

    return 0;
}

void PluginLoader::setPluginDir(string path) {
    this->pluginDir = path;
};