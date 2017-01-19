#include <iostream>
#include <thread>
#include <csignal>
#include <dlfcn.h>
#include <stdlib.h>
#include <dirent.h>

#include "Plugin.hpp"
#include "PluginHandler.hpp"
#include "InterruptHandle.hpp"
#include "PluginLoader.hpp"
#include "Logger.h"

#define RECV_TIMEOUT 2000

using namespace std;

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

string getPluginDir() {
    char *pluginDir;
    pluginDir = getenv("PLUGIN_DIR");

    if (pluginDir == NULL) {
        cerr << "No plugin directory passed. Falling back to the current directory." << endl;
        return ".";
    } else
        return pluginDir;
}

int getPlugins(string dir, vector<string> &files) {
    DIR *dp;
    struct dirent *dirp;
    if ((dp = opendir(dir.c_str())) == NULL) {
        cerr << "Error(" << errno << ") opening plugin directory " << dir << endl;
        return errno;
    }

    string prefix = "libsh";
    string name = "";
    while ((dirp = readdir(dp)) != NULL) {
        name = dirp->d_name;
        if (dirp->d_type != DT_REG || name.substr(0, 5) != prefix) continue;
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

int main() {
    custom("Load", "GPIO Plugin v0.1");
    error("What is going on here");
    warn("Help me?");
    info("I'm different");
    debug("This is kinda funny");
    trace("Ouh well...stuck in terminal again.");

    signal(SIGINT, onInterrupt);

    vector<string> plugins;
    string pluginDir = getPluginDir();
    getPlugins(pluginDir, plugins);

    if (plugins.size() == 0) {
        cerr << "No plugins found!" << endl;
        return 1;
    }

    PluginLoader pluginLoader("224.0.0.1", 1337);
    pluginLoader.setPluginDir(pluginDir);

    for (string plugin : plugins) {
        cout << "Loading plugin '" << plugin << "' . . ." << endl;
        pluginLoader.loadPlugin(plugin);
    }

    InterruptHandle handle;
    thread networking(receiveData, &pluginLoader.pluginHandler, &handle);
    thread processing(processData, &pluginLoader.pluginHandler, &handle);

    cout << "Awaiting requests . . ." << endl;
    while (interrupted == 0)
        sleep(1);

    cout << "\nInterrupted! Exiting..." << endl;
    handle.interrupt();
    networking.join();
    processing.join();

    return 0;
}