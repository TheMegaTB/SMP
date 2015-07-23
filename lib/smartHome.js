var path = require("path");
require('app-module-path').addPath(path.join(__dirname, '../'));

var AnyMesh = require('anymesh')
    , async = require('async')
    , fs = require('fs')
    , log = require('lib/log')()
    , benchmark = require('lib/benchmark');

var syncHandler = require('lib/sync')
    , settingsHandler = require('lib/settings')
    , sync = new syncHandler('settings/sync.json')
    , settings = new settingsHandler('settings/settings.json')
    , anyMesh = new AnyMesh()
    , syncPending = !settings.singleNode //Set singleNode only to true as long as this node is the only node in the network as it prevents sync at startup which might cause terrible desync issues
    , hostname = settings.hostname ? settings.hostname : ('node' + new Date().getTime());

var plugins = {};

var channels = {
    "sync": []
};

function startupMesh(cb) {
    anyMesh.networkID = settings.networkName;

    anyMesh.received = function (message) {
        log.mesh.info("MSG " + message.sender + " -> " + message.target);
        log.mesh.debug(message.data);

        //Call plugins for incoming channels
        for (var channel in channels) {
            if (channels.hasOwnProperty(channel)) {
                if (message.target == channel || message.data.channel == channel) {
                    channels[channel].forEach(function (callback) {
                        callback(message, sync[channel], settings);
                    });
                }
            }
        }

        //ToDo: Move this part to sync.js (handle updates in other instances)
        if (message.data.channel == 'sync' || message.target == 'sync') {
            var payload = message.data;
            if (payload.request == 'all') {
                log.mesh.debug("SNT sync");
                anyMesh.request(message.sender, {"channel": "sync", "data": sync});
            } else if (payload.request == 'device') {
                log.mesh.debug("SNT block " + payload.device);
                anyMesh.request(message.sender, {
                    "channel": "sync",
                    "data": sync[payload.device],
                    "device": payload.device
                });
            } else if (payload.request == null && payload.data) {
                if (payload.device) {
                    log.mesh.debug("RCV block");
                    sync[payload.device] = payload.data;
                } else {
                    log.mesh.debug("RCV sync");
                    for (var key in payload.data) if (payload.data.hasOwnProperty()) sync[key] = payload.data[key];
                }
                sync.save();
            }
        }
    };

    anyMesh.connectedTo = function (info) {
        log.mesh.info('CON ' + info.name);
        if (syncPending) {
            anyMesh.request(info.name, {"channel": "sync", "request": "all"});
            syncPending = false;
        }
        anyMesh.connections.forEach(function (socket) {
            socket.on('error', function (e) {
                if (e.code == 'ECONNRESET') log.mesh.error("ERR Connection reset " + socket.info.name);
                else log.mesh.error("ERR " + e.code);
            });
        });
    };

    anyMesh.disconnectedFrom = function (name) {
        log.mesh.info('DSC ' + name);
    };

    anyMesh.connect(hostname, Object.getOwnPropertyNames(channels).sort());
    log.mesh.debug("OK Mesh startup");
    if (typeof cb === 'function') cb();
}

function loadPlugins(cb) {
    var normalizedPath = path.join(__dirname, "../", "plugins");

    var files = fs.readdirSync(normalizedPath);
    async.each(files, function (file, cb) {
        var pluginName = file.substr(0, file.length - 3);

        log.plugin.debug("LOAD plugins/" + pluginName);
        plugins[pluginName] = require("plugins/" + pluginName);
        if (typeof plugins[pluginName].load === 'function') plugins[pluginName].load();

        async.forEachOf(plugins[pluginName].channels, function (func, name, callback) {
            if (channels[name] == null) channels[name] = [];
            channels[name] = channels[name].concat(func);
            callback();
        }, cb);

    }, function () {
        if (typeof cb === 'function') cb();
    });
}

async.series([
        loadPlugins,
        startupMesh
    ],
    function (err) {
        if (!err) {
            log.generic.warn("RDY " + hostname);
        } else {
            log.generic.error("ERR @async series");
            log.generic.error(err);
        }
    });