var Benchmark = require('benchmark')
    , ping = require('jjg-ping')
    , log = require('lib/log')();

module.exports = {
    performance: function (cb) {
        var suite = new Benchmark.Suite;
        suite.add('RegExp#test', function () {
            /o/.test('Hello World!');
        }).on('complete', function () {
            var result = Math.round(this[0].hz);
            log.benchmark.debug("OK Performance benchmark");
            log.benchmark.trace(result + " ops/sec");
            if (typeof cb === 'function') cb(null, result);
        }).run();
    },

    network: function (cb) {
        var p = {};
        ping.system.ping('192.168.178.1', function (latency, status) {
            if (status) p.router = latency;
            ping.system.ping('8.8.8.8', function (latency, status) {
                if (status) p.google = latency;
                log.benchmark.debug("OK Network benchmark");
                log.benchmark.trace(p.router + "ms -> Router");
                log.benchmark.trace(p.google + "ms -> Google");
                if (typeof cb === 'function') cb(null, p);
            });
        });
    }
};