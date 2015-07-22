var jsonf = require("jsonfile")
    , log = require('lib/log');
jsonf.spaces = 2;

module.exports = {
    writeSync: function (sync) {
        jsonf.writeFile('settings/sync.json', sync, function (err) {
            if (err) log.generic.error(err);
        });
    },

    writeSettings: function (settings) {
        jsonf.writeFile('settings/settings.json', settings, function (err) {
            if (err) log.generic.error(err);
        });
    },
    base64: {
        "decode": function (string) {
            return new Buffer(string, 'base64').toString('utf8');
        },
        "encode": function (string) {
            return new Buffer(string).toString('base64');
        }
    }
};