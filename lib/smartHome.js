var AnyMesh = require("anymesh")
    , async = require("async")
    , Benchmark = require('benchmark')
    , ping = require('jjg-ping');

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


function startupMesh() {
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

    anyMesh.connect("raspberry", ["events", "updates", "cookies"]);
}

async.series([
        function (callback) {
            performanceBenchmark(function () {
                callback(null);
            });
        },
        function (callback) {
            networkBenchmark(function () {
                callback(null);
            });
        }
    ],
    function (err) {
        if (!err) {
            console.log(performance);
            startupMesh();
        } else {
            console.log(err);
        }
    });