var AnyMesh = require('anymesh')
    , async = require('async')
    , Benchmark = require('benchmark')
    , ping = require('jjg-ping')
    , bunyan = require('bunyan')
    , fs = require('fs')
    , jsonf = require("jsonfile");

jsonf.spaces = 2;

var sync = require('./sync.json')
    , settings = require('./settings.json')
    , anyMesh = new AnyMesh()
    , syncPending = true;

if (!fs.existsSync('./log')) {
    fs.mkdirSync('./log');
}

var log = {
    "mesh": bunyan.createLogger({
        name: 'mesh',
        streams: [
            {
                level: 'debug',
                stream: process.stdout
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
                level: 'warn',
                stream: process.stdout
            },
            {
                level: 'trace',
                path: './log/benchmark.log'
            }
        ]
    }),
    "generic": bunyan.createLogger({
        name: 'benchmark',
        streams: [
            {
                level: 'trace',
                stream: process.stdout
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
    anyMesh.networkID = "smartHome";

    anyMesh.received = function (message) {
        //message is object containing message info
        //including type, sender, target, and data
        log.mesh.debug("message (" + message.type + ") sent by " + message.sender + " @ " + message.target);
        log.mesh.debug(message.data);

        if (message.data.channel == 'sync' || message.target == 'sync') {
            var payload = message.data;
            if (payload.request == 'all') {
                log.mesh.debug("SENT SYNC ARRAY");
                anyMesh.request(message.sender, {"channel": "sync", "data": sync});
            } else if (payload.request == 'device') {
                log.mesh.debug("SENT SYNC BLOCK " + payload.device);
                anyMesh.request(message.sender, {
                    "channel": "sync",
                    "data": sync[payload.device],
                    "device": payload.device
                });
            } else if (payload.request == null && payload.data) {
                if (payload.device) {
                    log.mesh.debug("RECEIVED SYNC BLOCK");
                    sync[payload.device] = payload.data;
                } else {
                    log.mesh.debug("RECEIVED SYNC ARRAY");
                    sync = payload.data;
                }
                //ToDo: Save json object to sync.json
                log.mesh.debug(sync);
                jsonf.writeFile('lib/sync.json', sync, function (err) {
                    if (err) log.mesh.error(err);
                });
            }
        }
    };

    anyMesh.connectedTo = function (info) {
        log.mesh.debug('Connected to ' + info.name);
        if (syncPending) {
            anyMesh.request(info.name, {"channel": "sync", "request": "all"});
            /*setTimeout(function () {
             anyMesh.publish("sync", {"device": "wifi", "data": sync["wifi"]});
             }, 10000);*/
            syncPending = false;
        }
    };

    anyMesh.disconnectedFrom = function (name) {
        log.mesh.debug('Disconnected from ' + name);
    };

    anyMesh.connect(settings.hostname + Math.floor((Math.random() * 100) + 1), settings.keywords);
    if (typeof cb === 'function') cb();
}

/*function syncSettings(device) {
    if (!device) anyMesh.publish("sync", {"request": "all"});
    else if (typeof device === 'String') anyMesh.publish("sync", {"request": "device", "device": device});
 }*/

async.series([
        function (callback) {
            performanceBenchmark(function () {
                log.benchmark.info("CPU benchmark completed");
                log.benchmark.debug(performance.regex + " ops/sec");
                callback(null);
            });
        },
        function (callback) {
            networkBenchmark(function () {
                log.benchmark.info("Network benchmark completed");
                log.benchmark.debug(performance.ping.router + "ms -> Router");
                log.benchmark.debug(performance.ping.google + "ms -> Google");
                callback(null);
            });
        },
        function (callback) {
            startupMesh(function () {
                log.mesh.info("Mesh startup completed");
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
            log.generic.info("Node ready for operation.");
        } else {
            log.generic.error("Error in async series!");
            log.generic.error(err);
        }
    });