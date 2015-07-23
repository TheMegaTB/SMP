var log = require('lib/log')()
    , helpers = require('lib/helpers')
    , syncblock = null
    , settings = null;

module.exports = {
    channels: {
        rename: function (message) {
            log.plugin.debug("rename received");
            if (message.data.name) settings.hostname = message.data.name;
            settings.save();
        }
    },
    rename: function (currentID, newID) {
        anyMesh.request(currentID, {"channel": "rename", "name": newID});
    },
    load: function (sync, conf, callback) {
        syncblock = sync;
        settings = conf;
        callback(true);
    }
};