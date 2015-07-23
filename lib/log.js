var bunyan = require('bunyan')
    , PrettyStream = require('bunyan-prettystream')
    , prettyStdOut = new PrettyStream()
    , argv = require('minimist')(process.argv.slice(2), opts = {})
    , fs = require('fs')
    , settings = require('./../settings/settings.json');

prettyStdOut.pipe(process.stdout);

if (!fs.existsSync('./log')) {
    fs.mkdirSync('./log');
}

module.exports = function (logLevel) {
    if (!logLevel) logLevel = settings.debugLevel;
    if (argv.debug) logLevel = argv.debug;
    return {
        mesh: bunyan.createLogger({
            name: 'mesh',
            streams: [
                {
                    level: logLevel,
                    type: 'raw',
                    stream: prettyStdOut
                },
                {
                    level: 'trace',
                    path: './log/mesh.log'
                }
            ]
        }),
        benchmark: bunyan.createLogger({
            name: 'benchmark',
            streams: [
                {
                    level: logLevel,
                    type: 'raw',
                    stream: prettyStdOut
                },
                {
                    level: 'trace',
                    path: './log/benchmark.log'
                }
            ]
        }),
        generic: bunyan.createLogger({
            name: 'generic',
            streams: [
                {
                    level: logLevel,
                    type: 'raw',
                    stream: prettyStdOut
                },
                {
                    level: 'trace',
                    path: './log/generic.log'
                }
            ]
        }),
        plugin: bunyan.createLogger({
            name: 'plugin',
            streams: [
                {
                    level: logLevel,
                    type: 'raw',
                    stream: prettyStdOut
                },
                {
                    level: 'trace',
                    path: './log/plugin.log'
                }
            ]
        })
    }
};