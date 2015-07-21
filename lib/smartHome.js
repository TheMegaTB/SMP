var AnyMesh = require('anymesh')
    , async = require('async')
    , Benchmark = require('benchmark')
    , ping = require('jjg-ping')
    , settings = require('./settings.json')
    , bunyan = require('bunyan')
    , fs = require('fs');

if (!fs.existsSync('./log')) {
    fs.mkdirSync('./log');
}

var log = {
    "mesh": bunyan.createLogger({
        name: 'mesh',
        streams: [
            {
                level: 'warn',
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
    var anyMesh = new AnyMesh();

    anyMesh.received = function (message) {
        //message is object containing message info
        //including type, sender, target, and data
        console.log("message sent by " + message.sender);
        console.log(message.data);
    };

    anyMesh.connectedTo = function (info) {
        console.log('Connected to ' + info.name);
        if (info.name == "mba") anyMesh.request("mba", {"msg": "Hello Bob", "priority": 1});
    };

    anyMesh.disconnectedFrom = function (name) {
        console.log('Disconnected from ' + name);
    };

    anyMesh.connect(settings.hostname, settings.keywords);
    if (typeof cb === 'function') cb();
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