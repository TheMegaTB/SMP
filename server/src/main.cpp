#include <iostream>
#include <csignal>
#include <dirent.h>

#include "Plugin.hpp"
#include "PluginHandler.hpp"
#include "InterruptHandle.hpp"
#include "PluginLoader.hpp"

#define RECV_TIMEOUT 2000

using namespace std;

void sendData(PluginHandler *p, InterruptHandle *handle) {
    while (!(*handle).isInterrupted()) {
        p->sendData();
        usleep(200000); // 200ms
    }
}

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
        warn("No plugin directory passed. Falling back to the current directory.");
        return ".";
    } else
        return pluginDir;
}

int getPlugins(string pluginDir, string dir, vector<string> &files) {
    DIR *dp;
    struct dirent *dirp;

    string cwd = pluginDir + "/" + dir;

    if ((dp = opendir(cwd.c_str())) == NULL) {
        err("Error opening plugin (sub-)directory");
        err(cwd);
        return errno;
    }

    string prefix = "libsmp-";
    string name = "";
    while ((dirp = readdir(dp)) != NULL) {
        name = dirp->d_name;

        // Recursively search through directories
        if (dirp->d_type == DT_DIR && !(name == "." || name == "..")) {
            getPlugins(pluginDir, dir + "/" + name, files);
            continue;
        }

        // Skip all non-files and files that do not have the correct prefix
        if (dirp->d_type != DT_REG || name.substr(0, 7) != prefix) continue;

        // Push the remaining files into the list
        files.push_back(dir + "/" + string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

#include "version.h"
int main() {
    info("SMP v" + gGIT_VERSION_SHORT);

    signal(SIGINT, onInterrupt);

    vector<string> plugins;
    string pluginDir = getPluginDir();
    getPlugins(pluginDir, ".", plugins);

    if (plugins.size() == 0) {
        err("No plugins found!");
        return 1;
    }

    PluginLoader pluginLoader("224.0.0.1", 1337);
    pluginLoader.setPluginDir(pluginDir);

    for (string plugin : plugins) {
        pluginLoader.loadPlugin(plugin);
    }

    info("Plugins locked and loaded. Awaiting requests ...");

    // Use this if you are debugging since it is way easier to debug without threads
    while (interrupted == 0) {
        pluginLoader.pluginHandler.sendData();
        pluginLoader.pluginHandler.receiveData(RECV_TIMEOUT);
        pluginLoader.pluginHandler.processData();
    }

//    InterruptHandle handle;
//    thread networkingOut(sendData, &pluginLoader.pluginHandler, &handle);
//    thread networkingIn(receiveData, &pluginLoader.pluginHandler, &handle);
//    thread processing(processData, &pluginLoader.pluginHandler, &handle);
//
//    while (interrupted == 0)
//        sleep(1);
//
//    warn("Interrupted. Exiting ...");
//    handle.interrupt();
//    networkingOut.join();
//    networkingIn.join();
//    processing.join();

    return 0;
}