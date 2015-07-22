var AnyMesh = require('anymesh')
    , async = require('async')
    , Benchmark = require('benchmark')
    , ping = require('jjg-ping')
    , bunyan = require('bunyan')
    , fs = require('fs')
    , jsonf = require("jsonfile")
    , PrettyStream = require('bunyan-prettystream')
    , prettyStdOut = new PrettyStream();

jsonf.spaces = 2;
prettyStdOut.pipe(process.stdout);

var sync = require('./../settings/sync.json')
    , settings = require('./../settings/settings.json')
    , anyMesh = new AnyMesh()
    , syncPending = !settings.singleNode //Set singleNode only to true as long as this node is the only node in the network as it prevents sync at startup which might cause terrible desync issues
    , hostname = settings.hostname ? settings.hostname : ('node' + new Date().getTime());

if (!fs.existsSync('./log')) {
    fs.mkdirSync('./log');
}

var log = {
    "mesh": bunyan.createLogger({
        name: 'mesh',
        streams: [
            {
                level: settings.debugLevel,
                type: 'raw',
                stream: prettyStdOut
            },
            {
                level: 'trace',
                path: './log/mesh.log'
            }
        ]
    }),
    "benchmark": bunyan.createLogger({
        name: 'benchmark',
        streams: [
            {
                level: settings.debugLevel,
                type: 'raw',
                stream: prettyStdOut
            },
            {
                level: 'trace',
                path: './log/benchmark.log'
            }
        ]
    }),
    "generic": bunyan.createLogger({
        name: 'generic',
        streams: [
            {
                level: settings.debugLevel,
                type: 'raw',
                stream: prettyStdOut
            },
            {
                level: 'trace',
                path: './log/generic.log'
            }
        ]
    })
};

var performance = {
    "regex": null,
    "ping": {
        "router": null,
        "google": null
    }
};

var base64 = {
    "decode": function (string) {
        return new Buffer(string, 'base64').toString('utf8');
    },
    "encode": function (string) {
        return new Buffer(string).toString('base64');
    }
};

function writeSync() {
    jsonf.writeFile('settings/sync.json', sync, function (err) {
        if (err) log.generic.error(err);
    });
}

function writeSettings() {
    jsonf.writeFile('settings/settings.json', settings, function (err) {
        if (err) log.generic.error(err);
    });
}

function performanceBenchmark(cb) {
    var suite = new Benchmark.Suite;
    suite.add('RegExp#test', function () {
        /o/.test('Hello World!');
    }).on('complete', function () {
        var result = this[0];
        performance.regex = Math.round(result.hz);
        if (typeof cb === 'function') cb();
    }).run();
}

function networkBenchmark(cb) {
    ping.system.ping('192.168.178.1', function (latency, status) {
        if (status) performance.ping.router = latency;
        ping.system.ping('8.8.8.8', function (latency, status) {
            if (status) performance.ping.google = latency;
            if (typeof cb === 'function') cb();
        });
    });
}

function startupMesh(cb) {
    anyMesh.networkID = settings.networkName;

    anyMesh.received = function (message) {
        //message is object containing message info
        //including type, sender, target, and data
        log.mesh.info("MSG " + message.sender + " -> " + message.target);
        log.mesh.debug(message.data);

        if (message.data.channel == 'rename') {
            if (message.data.name) settings.hostname = message.data.name;
            writeSettings();
        }

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
                writeSync();
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

    anyMesh.connect(hostname, settings.plugins);
    if (typeof cb === 'function') cb();
}

async.series([
        function (callback) {
            performanceBenchmark(function () {
                log.benchmark.debug("OK Performance benchmark");
                log.benchmark.trace(performance.regex + " ops/sec");
                callback(null);
            });
        },
        function (callback) {
            networkBenchmark(function () {
                log.benchmark.debug("OK Network benchmark");
                log.benchmark.trace(performance.ping.router + "ms -> Router");
                log.benchmark.trace(performance.ping.google + "ms -> Google");
                callback(null);
            });
        },
        function (callback) {
            startupMesh(function () {
                log.mesh.debug("OK Mesh startup");
                callback(null);
            })
        },
        function (callback) {
            //Do stuff with mesh network aka send benchmark results etc.
            callback(null);
        }
    ],
    function (err) {
        if (!err) {
            log.generic.info("RDY " + hostname);
        } else {
            log.generic.error("ERR @async series");
            log.generic.error(err);
        }
    });