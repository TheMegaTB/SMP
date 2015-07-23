var log = require('lib/log')()
    , helpers = require('lib/helpers');

module.exports = {
    channels: {
        web: function (message, syncblock, settings) {
            log.plugin.trace("web received");
        }
    },
    load: function () {
        log.plugin.trace("webapp setup called");
    }
};