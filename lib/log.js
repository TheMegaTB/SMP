var bunyan = require('bunyan')
    , PrettyStream = require('bunyan-prettystream')
    , prettyStdOut = new PrettyStream()
    , settings = require('./../settings/settings.json');

prettyStdOut.pipe(process.stdout);

module.exports = {
    mesh: bunyan.createLogger({
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
    benchmark: bunyan.createLogger({
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
    generic: bunyan.createLogger({
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