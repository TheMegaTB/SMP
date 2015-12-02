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

function Logger(name, logLevel) {
    return bunyan.createLogger({
        name: name,
        streams: [
            {
                level: logLevel,
                type: 'raw',
                stream: prettyStdOut
            },
            {
                level: 'trace',
                path: './log/' + name + '.log'
            }
        ]
    });
}

module.exports = function (logLevel) {
    if (!logLevel) logLevel = settings.debugLevel;
    if (argv.debug) logLevel = argv.debug;
    return {
        mesh: new Logger('mesh', logLevel),
        benchmark: new Logger('benchmark', logLevel),
        generic: new Logger('generic', logLevel),
        plugin: new Logger('plugin', logLevel)
    }
};