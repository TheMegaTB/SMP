import {log} from "debug/browser";

const ws = new WebSocket("ws://127.0.0.1:3003/");

export const devices = [];

function send(data) {
    ws.send(JSON.stringify(data));
}

function updateDevices() {
    devices.length = 0; // Clear all devices
    send({action: "query"});
}

ws.onmessage = function (event) {
    const data = JSON.parse(event.data);

    if (data.action == "announce") {
        devices.push(data);
    }

    // console.log("MSG:", JSON.parse(event.data));
};

ws.onopen = function (event) {
    log("Connection established!");
    updateDevices();
};