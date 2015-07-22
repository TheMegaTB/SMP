var AnyMesh = require('anymesh')
    , async = require('async')
    , Benchmark = require('benchmark')
    , ping = require('jjg-ping')
    , bunyan = require('bunyan')
    , fs = require('fs')
    , jsonf = require("jsonfile");

var sync = require('./sync.json')
    , settings = require('./settings.json')
    , anyMesh = new AnyMesh();

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
        log.mesh.debug("message sent by " + message.sender);
        log.mesh.debug(message.data);

        if (message.target == 'sync') {
            if (message.data.request == 'all') anyMesh.request(message.sender, sync);
            else anyMesh.request(message.sender, sync[message.data.request]);
        }
    };

    anyMesh.connectedTo = function (info) {
        log.mesh.debug('Connected to ' + info.name);
        syncSettings();
    };

    anyMesh.disconnectedFrom = function (name) {
        log.mesh.debug('Disconnected from ' + name);
    };

    anyMesh.connect(settings.hostname + Math.floor((Math.random() * 100) + 1), settings.keywords);
    if (typeof cb === 'function') cb();
}

function syncSettings(key) {
    if (!key) anyMesh.publish("sync", {"request": "all"});
    else if (typeof key === 'String') anyMesh.publish("sync", {"request": key})
}

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