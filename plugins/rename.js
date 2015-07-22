var log = require('lib/log')()
    , helpers = require('lib/helpers');

module.exports = {
    channels: {
        rename: function (message) {
            log.mesh.info("rename received");
            if (message.data.name) settings.hostname = message.data.name;
            helpers.writeSettings(settings);
        }
    },
    rename: function (currentID, newID) {
        anyMesh.request(currentID, {"channel": "rename", "name": newID});
    }
};