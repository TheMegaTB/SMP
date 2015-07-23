var log = require('lib/log')()
    , helpers = require('lib/helpers');

module.exports = {
    channels: {
        rename: function (message, syncblock, settings) {
            log.plugin.debug("rename received");
            if (message.data.name) settings.hostname = message.data.name;
            settings.save();
        }
    },
    rename: function (currentID, newID) {
        anyMesh.request(currentID, {"channel": "rename", "name": newID});
    }
};