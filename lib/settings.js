var jsonf = require("jsonfile")
    , log = require('lib/log')();

module.exports = function (filepath) {
    var json = jsonf.readFileSync(filepath);
    json.save = function () {
        jsonf.writeFile(filepath, json, function (err) {
            if (err) log.generic.error(err);
        });
    };
    return json;
};