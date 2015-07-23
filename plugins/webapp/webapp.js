var log = require('lib/log')()
    , net = require('net')
    , path = require('path')
    , spawn = require('child_process').spawn
    , meteor = null;

process.on('exit', function () {
    meteor.kill();
});

module.exports = {
    channels: {
        web: function (message, syncblock, settings) {
            log.plugin.trace("WEB received message");
        }
    },
    load: function () {
        var server = net.createServer(function (c) { //'connection' listener
            //console.log('client connected');
            c.on('end', function () {
                //console.log('client disconnected');
            });
            c.write('hello\r\n');
            c.pipe(c);
        });
        server.listen(8124, function () { //'listening' listener
            log.plugin.debug('WEB TCP Server online');
            spawn('meteor', [], {cwd: path.join(__dirname, 'meteor')});
        });
    }
};