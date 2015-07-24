var log = require('lib/log')()
    , async = require('async')
    , fs = require('fs')
    , GPIO = require('wiringPiHandler')
    , gpio = {};

//7 = Screens (s1) = 26
//8 = Vent (s2) = 24
//25 = desk light (s3) = 22
//24 = bed light (s4) = 18
//23 = sound system (s5) = 15

//9 = 21
//10 = 19
//11 = 23

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
            var gpios = [7, 8, 9, 10, 11, 23, 24, 25]; //[15, 18, 19, 21, 22, 23, 24, 26];
            async.each(gpios, function (pin, cb) {
                gpio[g] = new GPIO(pin);
                cb();
            }, function (err) {
                log.plugin.warn("Setting timeout");
                setTimeout(function () {
                    console.log("ON");
                    gpio[18].set(0);
                }, 5000);
                callback(!err);
            });
        } else {
            log.plugin.debug("ENOENT, no such directory '/sys/class/gpio'");
            callback(false);
        }
    },
    unload: function (callback) {
        async.forEachOf(gpio, function (g, id, cb) {
            g.reset();
            cb();
        }, function () {
            log.plugin.trace("unexported gpios");
            callback();
        });
    }
};