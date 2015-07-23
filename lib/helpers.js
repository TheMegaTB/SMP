var jsonf = require("jsonfile")
    , log = require('lib/log')();
jsonf.spaces = 2;

module.exports = {
    base64: {
        "decode": function (string) {
            return new Buffer(string, 'base64').toString('utf8');
        },
        "encode": function (string) {
            return new Buffer(string).toString('base64');
        }
    }
};