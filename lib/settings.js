var jsonf = require("jsonfile")
    , log = require('lib/log')();

jsonf.spaces = 2;

module.exports = function (filepath) {
    var json = jsonf.readFileSync(filepath);
    json.save = function (cb) {
        jsonf.writeFile(filepath, json, function (err) {
            if (err) log.generic.error(err);
            if (typeof cb === 'function') cb();
        });
    };
    return json;
};