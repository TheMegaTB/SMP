var path = path = require("path");
require('app-module-path').addPath(path.join(__dirname, '../'));

var AnyMesh = require('anymesh')
    , async = require('async')
    , fs = require('fs')
    , helpers = require('lib/helpers')
    , log = require('lib/log')
    , benchmark = require('lib/benchmark');

var sync = require('./../settings/sync.json')
    , settings = require('./../settings/settings.json')
    , anyMesh = new AnyMesh()
    , syncPending = !settings.singleNode //Set singleNode only to true as long as this node is the only node in the network as it prevents sync at startup which might cause terrible desync issues
    , hostname = settings.hostname ? settings.hostname : ('node' + new Date().getTime());

if (!fs.existsSync('./log')) {
    fs.mkdirSync('./log');
}

var performance = {
    "regex": null,
    "ping": {
        "router": null,
        "google": null
    }
};

var plugins = {};

var channels = {
    "sync": []
};

function startupMesh(cb) {
    anyMesh.networkID = settings.networkName;

    anyMesh.received = function (message) {
        //message is object containing message info
        //including type, sender, target, and data
        log.mesh.info("MSG " + message.sender + " -> " + message.target);
        log.mesh.debug(message.data);

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
                    sync = payload.data;
                }
                helpers.writeSync(sync);
            }
        }
    };

    anyMesh.connectedTo = function (info) {
        log.mesh.info('CON ' + info.name);
        if (syncPending) {
            anyMesh.request(info.name, {"channel": "sync", "request": "all"});
            syncPending = false;
        }
    };

    anyMesh.disconnectedFrom = function (name) {
        log.mesh.info('DSC ' + name);
    };

    anyMesh.connect(hostname, channels);
    log.mesh.debug("OK Mesh startup");
    if (typeof cb === 'function') cb();
}

function loadPlugins(cb) {
    var normalizedPath = path.join(__dirname, "../", "plugins");

    var files = fs.readdirSync(normalizedPath);
    async.each(files, function (file, cb) {
        var pluginName = file.substr(0, file.length - 3);

        plugins[pluginName] = require("plugins/" + pluginName);
        plugins[pluginName].setLogger = log.mesh;

        async.forEachOf(plugins[pluginName].channels, function (func, name, callback) {
            if (channels[name] == null) channels[name] = [];
            channels[name] = channels[name].concat(func);
            callback();
        }, cb);

    }, function () {
        if (typeof cb === 'function') cb();
    });
}

//benchmark.performance,
//benchmark.network,
async.series([
        loadPlugins,
        startupMesh
    ],
    function (err, results) {
        if (!err) {
            //performance.regex = results[0];
            //performance.ping = results[1];
            log.generic.info("RDY " + hostname);
        } else {
            log.generic.error("ERR @async series");
            log.generic.error(err);
        }
    });