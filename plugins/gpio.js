var log = require('lib/log')()
    , async = require('async')
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
        var gpios = [7, 8, 9, 10, 11, 23, 24, 25];
        async.each(gpios, function (g, cb) {
            log.plugin.trace(g);
            gpio[g] = gpioLib.export(g, "in", function () {
                gpio[g].set(1, function () {
                    gpio[g].setDirection("out");
                    cb();
                });
            });
        }, function (err) {
            log.plugin.debug("Initialized GPIOs");
        });
    }
};