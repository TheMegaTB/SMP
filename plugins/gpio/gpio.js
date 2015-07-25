var log = require('lib/log')()
    , async = require('async')
    , fs = require('fs')
    , hasbin = require('hasbin')
    , GPIO = require('plugins/gpio/wiringPiHandler')
    , gpio = {}
    , sync = null
    , settings = null;

//7 = Screens (s1) = 26
//8 = Vent (s2) = 24
//25 = desk light (s3) = 22
//24 = bed light (s4) = 18
//23 = sound system (s5) = 15

//9 = 21
//10 = 19
//11 = 23

module.exports = {
    load: function (s, conf, callback) {
        sync = s;
        settings = conf;

        if (sync.gpios == undefined) {
            sync.gpios = {};
            //Debug purposes
            sync.gpios[7] = 1;
            sync.gpios[8] = 0;
            sync.gpios[9] = 1;
            sync.gpios[10] = 1;
            sync.gpios[11] = 1;
            sync.gpios[23] = 1;
            sync.gpios[24] = 0;
            sync.gpios[25] = 1;
            //End debug purposes
        }

        try {
            var p = fs.lstatSync('/sys/class/gpio');
        } catch (err) {
            log.plugin.debug(err);
            callback(false);
            return;
        }

        hasbin('gpio', function (binaryExists) {
            if (p.isDirectory() && binaryExists) {
                var gpios = [7, 8, 9, 10, 11, 23, 24, 25]; //List of pins that are connected
                async.each(gpios, function (pin, cb) {
                    gpio[pin] = new GPIO(pin, sync.gpios);
                    cb();
                }, function (err) {
                    setTimeout(function () {
                        log.plugin.debug("GPIO 8 = OFF");
                        gpio[8].set(1, sync.gpios);
                    }, 5000);
                    callback(!err);
                });
            } else if (binaryExists) {
                log.plugin.debug("ENOENT, no such directory '/sys/class/gpio'");
                callback(false);
            } else {
                log.plugin.debug("ENOENT, no such binary 'gpio'");
                callback(false);
            }
        });
    },
    unload: function (callback) {
        async.forEachOf(gpio, function (g, id, cb) {
            g.reset();
            cb();
        }, function () {
            callback();
        });
    }
};