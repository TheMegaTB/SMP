if (Meteor.isClient) {
    // counter starts at 0
    Session.setDefault('counter', 0);

    Template.hello.helpers({
        counter: function () {
            return Session.get('counter');
        }
    });

    Template.hello.events({
        'click button': function () {
            // increment the counter when button is clicked
            Session.set('counter', Session.get('counter') + 1);
        }
    });
}

if (Meteor.isServer) {
    var path = Npm.require("path");
    var net = Npm.require("net");
    Meteor.startup(function () {
        // code to run on server at startup
        var client = net.connect({port: 8124},
            function () { //'connect' listener
                console.log('connected to server!');
                client.write('world!\r\n');
            });
        client.on('data', function (data) {
            console.log(data.toString());
            client.end();
        });
        client.on('end', function () {
            console.log('disconnected from server');
        });
    });
}
