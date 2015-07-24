var log = require('lib/log')()
    , async = require('async')
    , fs = require('fs')
    , gpioLib = require('gpio')
    , gpio = {};

//7 = Screens (s1)
//8 = Vent (s2)
//25 = desk light (s3)
//24 = bed light (s4)
//23 = sound system (s5)

//7
//8
//9
//10

module.exports = {
    load: function (sync, settings, callback) {
        try {
            var p = fs.lstatSync('/sys/class/gpio');
        } catch (err) {
            log.plugin.debug(err);
            callback(false);
            return;
        }

        if (p.isDirectory()) {
            var gpios = [7, 8, 9, 10, 11, 23, 24, 25];
            async.each(gpios, function (g, cb) {
                log.plugin.trace(g);
                log.plugin.trace(typeof cb);
                gpio[g] = gpioLib.export(g, {
                    direction: "in",
                    ready: function () {
                        gpio[g].set(0, function () {
                            gpio[g].setDirection("out");
                            cb();
                        });
                    }
                });
            }, function (err) {
                log.plugin.debug("Initialized GPIOs");
                callback(true);
            });
        } else {
            log.plugin.debug("ENOENT, no such directory '/sys/class/gpio'");
            callback(false);
        }
    }
};