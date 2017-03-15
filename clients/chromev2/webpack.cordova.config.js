var webpack = require("webpack"),
    path = require("path"),
    fileSystem = require("fs"),
    env = require("./utils/env"),
    HtmlWebpackPlugin = require("html-webpack-plugin"),
    WriteFilePlugin = require("write-file-webpack-plugin"),
    CordovaPlugin = require('webpack-cordova-plugin');


// load the secrets
var alias = {};

var secretsPath = path.join(__dirname, ("secrets." + env.NODE_ENV + ".js"));

if (fileSystem.existsSync(secretsPath)) {
    alias["secrets"] = secretsPath;
}

module.exports = {
    entry: {
        vendors: [
            "webpack-material-design-icons"
        ],
        popup: path.join(__dirname, "src", "js", "popup", "popup.jsx")
    },
    output: {
        path: path.join(__dirname, "build"),
        filename: "[name].bundle.js"
    },
    module: {
        loaders: [
            {test: /\.(js|jsx)$/, loader: "babel"},
            {test: /\.js$/, loader: 'exports-loader'},
            {test: /\.(jpe?g|png|gif|svg|eot|woff|ttf|svg|woff2)$/, loader: "file?name=[name].[ext]"},
            {test: /\.css$/, loaders: ["style", "css"]}
        ]
    },
    resolve: {
        alias: alias,
        extensions: ["", ".js", ".jsx", ".css"],
        modulesDirectories: ["web_modules", "node_modules", "bower_components"]
    },
    plugins: [
        // expose and write the allowed env vars on the compiled bundle
        new webpack.DefinePlugin({"process.env": JSON.stringify(env)}),
        new HtmlWebpackPlugin({
            template: path.join(__dirname, "src", "popup.html"),
            filename: "popup.html",
            chunks: ["popup"]
        }),
        new WriteFilePlugin(),
        new CordovaPlugin({
            config: 'config.xml',  // Location of Cordova' config.xml (will be created if not found)
            src: 'popup.html',     // Set entry-point of cordova in config.xml
            platform: 'android',   // Set `webpack-dev-server` to correct `contentBase` to use Cordova plugins.
            version: true,         // Set config.xml' version. (true = use version from package.json)
        })
    ]
};
