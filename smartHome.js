var AnyMesh = require("anymesh")
    , async = require("async")
    , Benchmark = require('benchmark');

var suite = new Benchmark.Suite;
suite.add('RegExp#test', function () {
    /o/.test('Hello World!');
})
    .add('String#indexOf', function () {
        'Hello World!'.indexOf('o') > -1;
    })
// add listeners
    .on('cycle', function (event) {
        console.log(String(event.target));
    })
    .on('complete', function () {
        console.log('Fastest is ' + this.filter('fastest').pluck('name'));
    })
// run async
    .run({'async': false});

var anyMesh = new AnyMesh();

anyMesh.received = function (message) {
    //message is object containing message info
    //including type, sender, target, and data
    console.log("message sent by " + message.sender);
    console.log(message.data);
};

anyMesh.connectedTo = function (info) {
    console.log('Connected to ' + info.name);
    if (info.name == "mba") anyMesh.request("mba", {"msg": "Hello Bob", "priority": 1});
};

anyMesh.disconnectedFrom = function (name) {
    console.log('Disconnected from ' + name);
};

anyMesh.connect("raspberry", ["events", "updates", "cookies"]);

setTimeout(function () {

}, 5000);