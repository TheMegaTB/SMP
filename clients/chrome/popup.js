var ws = new WebSocket("ws://127.0.0.1:3003/");

var devices = [];

function send(data) {
    ws.send(JSON.stringify(data));
}

function updateDevices() {
    devices = [];
    send({action: "query"});
}

function createFixture(icon, name, channel) {
    channel = channel[0] + channel[1] + channel[2];
    return '' +
        '<li class="mdl-list__item">' +
            '<span class="mdl-list__item-primary-content">' +
                '<i class="icon material-icons">' + icon + '</i>' +
                name +
            '</span>' +
            '<span class="mdl-list__item-secondary-action">' +
                '<p style="width:300px"><input class="mdl-slider mdl-js-slider" id="slider-' + channel + '" type="range" min="0" max="100" tabindex="0"></p>' +
            '</span>' +
        '</li>';
}

function insertFixture(fixture) {
    document.getElementById("fixtures").insertAdjacentHTML( 'afterbegin', fixture );
}

ws.onmessage = function (event) {
    var data = JSON.parse(event.data);

    if (data.action == "announce") {
        devices.push(data);
    }

    // console.log("MSG:", JSON.parse(event.data));
};

ws.onopen = function (event) {
    console.log("Connection established!");
    updateDevices();
};