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
    , hostname = settings.hostname ? settings.hostname : ('node' + new Date().getTime())
    , shuttingDown = false;

var plugins = {};

var channels = {
    "sync": []
};

//process.stdin.resume();

function exitHandler(err) {
    if (shuttingDown) return;
    shuttingDown = true;
    unloadPlugins(function () {
        log.generic.warn("EXIT " + hostname);
        if (err) throw new Error(err);
        else process.exit();
    });
}
//do something when app is closing
process.on('exit', exitHandler);

//catches ctrl+c event
process.on('SIGINT', exitHandler);

//catches uncaught exceptions
process.on('uncaughtException', exitHandler);

function startupMesh(cb) {
    anyMesh.networkID = settings.networkName;

    anyMesh.received = function (message) {
        log.mesh.debug("MSG " + message.sender + " -> " + message.target);
        log.mesh.trace(message.data);

        //Call plugins for incoming channels
        for (var channel in channels) {
            if (channels.hasOwnProperty(channel)) {
                if (message.target == channel || message.data.channel == channel) {
                    channels[channel].forEach(function (callback) {
                        callback(message);
                    });
                }
            }
        }

        //ToDo: Move this part to sync.js (handle updates in other instances)
        if (message.data.channel == 'sync' || message.target == 'sync') {
            var payload = message.data;
            if (payload.request == 'all') {
                log.mesh.trace("SNT sync");
                anyMesh.request(message.sender, {"channel": "sync", "data": sync});
            } else if (payload.request == 'device') {
                log.mesh.trace("SNT block " + payload.device);
                anyMesh.request(message.sender, {
                    "channel": "sync",
                    "data": sync[payload.device],
                    "device": payload.device
                });
            } else if (payload.request == null && payload.data) {
                if (payload.device) {
                    log.mesh.trace("RCV block");
                    sync[payload.device] = payload.data;
                } else {
                    log.mesh.trace("RCV sync");
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
            if (socket.info.name == info.name) {
                log.mesh.debug("ADD Error listener to " + socket.info.name);
                socket.on('error', function (e) {
                    if (e.code == 'ECONNRESET') log.mesh.error("ERR Connection reset " + socket.info.name);
                    else log.mesh.error("ERR " + e.code);
                });
            }
        });
    };

    anyMesh.disconnectedFrom = function (name) {
        log.mesh.info('DSC ' + name);
        anyMesh.connections.forEach(function (socket) {
            if (socket.info.name == name) {
                log.mesh.debug("REM Error listener from " + name);
                socket.removeAllListeners('error');
            }
        });
    };

    anyMesh.connect(hostname, Object.getOwnPropertyNames(channels).sort());
    log.mesh.debug("OK Mesh startup");
    if (typeof cb === 'function') cb();
}

function loadPlugins(cb) {
    var normalizedPath = path.join(__dirname, "../", "plugins");

    var files = fs.readdirSync(normalizedPath);

    async.each(files, function (file, cb) {
        var pluginName = file.replace('.js', '');

        log.plugin.info("LOAD plugins/" + pluginName);
        plugins[pluginName] = require("plugins/" + pluginName);
        var pluginCallback = function (success) {
            if (!success) {
                log.plugin.warn("ERR loading plugins/" + pluginName);
                cb();
            } else {
                async.forEachOf(plugins[pluginName].channels, function (func, name, callback) {
                    if (channels[name] == null) channels[name] = [];
                    channels[name] = channels[name].concat(func);
                    callback();
                }, cb);
            }
        };
        if (sync.plugins[pluginName] == undefined) sync.plugins[pluginName] = {};
        if (typeof plugins[pluginName].load === 'function') plugins[pluginName].load(sync.plugins[pluginName], settings, pluginCallback);
        else pluginCallback(true);

    }, function () {
        sync.save();
        if (typeof cb === 'function') cb();
    });
}

function unloadPlugins(cb) {
    async.forEachOf(plugins, function (plugin, name, cb) {
        log.plugin.info("UNLD plugins/" + name);
        if (typeof plugin.unload === 'function') plugin.unload(cb);
        else cb();
    }, function () {
        sync.save(function () {
            settings.save(function () {
                cb();
            });
        });
    });
}

async.series([
        loadPlugins,
        startupMesh
    ],
    function (err) {
        if (!err) {
            log.generic.warn("RDY " + hostname);
            setInterval(function () {
                sync.save();
                settings.save();
                log.generic.info("SAVE Settings/Sync");
            }, 300000); //autosave every 5min //TODO add timestamp to sync.json
        } else {
            log.generic.error("ERR @async series");
            log.generic.error(err);
        }
    });