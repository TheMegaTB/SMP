var webpack = require("webpack"),
    config = require("../webpack.cordova.config");

require("./prepare");

webpack(
    config,
    function (err) {
        if (err) throw err;
    }
);
