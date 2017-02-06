import {createStore} from "redux";
import {Socket} from "../websocket";

const defaultStore = {
    floors: {},
    devices: {},
    target: []
};

function channelToID(channel) {
    let id = "";
    for (let entry in channel) {
        if (!channel.hasOwnProperty(entry)) continue;
        id += channel[entry].toString();
    }

    return id;
}

function counter(state = defaultStore, action) {
    switch (action.type) {
        case 'NAVIGATE':
            state.target = action.target === "home" ? [] : action.target;
            break;
        case 'ADD_DEVICE':
            state.devices[channelToID(action.device.channel)] = action.device;
            break;
        case 'ADD_FLOOR':
            state.floors[action.floor.id] = action.floor;
            break;
        case 'FLUSH_DEVICES':
            state.devices = {};
    }

    return state;
}

export let store = createStore(counter);

store.subscribe(() =>
    console.log(store.getState())
);

store.dispatch({type: 'ADD_FLOOR', floor: {
    name: "Ground floor", id: 0, rooms: [
        { id: 0, name: "Living room", icon: "weekend" }
    ]
}});
store.dispatch({type: 'ADD_FLOOR', floor: {
    name: "First floor", id: 1, rooms: [
        { id: 0, name: "Corridor", icon: "blur_linear" },
        { id: 1, name: "Bedroom", icon: "hotel" }
    ]
}});

const ws = Socket;

function updateDevices() {
    store.dispatch({type: 'FLUSH_DEVICES'});
    ws.send(JSON.stringify({action: "query"}));
}

ws.onopen = function (event) {
    console.log("Connection established!");
    updateDevices();
};

ws.onmessage = function (event) {
    const data = JSON.parse(event.data);
    if (data.action == "announce") store.dispatch({type: 'ADD_DEVICE', device: data});
};

// store.dispatch({type: 'FLUSH'});