var Plugins = require('js-plugins');
var pluginManager = new Plugins();

pluginManager.scanSubdirs(["./plugins"]);
pluginManager.scan();

var host = {
    debug: true
}

pluginManager.connect(host, 'smartHome:actor', {multi: true}, function (err, outputs, names) {
    if (host.debug) {
        console.log("Connected plugins", names);
    }

    outputs.map(function (plugin) {
        plugin.onEvent(0, { target: "0/4/0", value: true });
    });

    // var names = outputs.map(function (plugin) {
    //     return plugin.getDisplayName();
    // });

    // console.log('Hello', names.join(', ').replace(/, ([^,]*)$/, ' and $1!'));
});

pluginManager.connect(host, 'smartHome:input', {multi:true}, function (err, outputs, names) {
    
});
