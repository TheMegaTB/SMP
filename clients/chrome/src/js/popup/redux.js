import {createStore} from "redux";

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
        case 'FLUSH':
            state = defaultStore;
    }

    return state;
}

export let store = createStore(counter);

store.subscribe(() =>
    console.log(store.getState())
);

store.dispatch({type: 'ADD_DEVICE', device: {action: "announce", attributes: ["binary","dimmable"], channel: [1,1,1], name:"Ceiling light", type:"Fixture"}});
store.dispatch({type: 'ADD_FLOOR', floor: {
    name: "Ground floor", id: 0, rooms: [
        { id: 0, name: "Living room", icon: "weekend" },
        { id: 1, name: "Corridor", icon: "blur_linear" }
    ]
}});
store.dispatch({type: 'ADD_FLOOR', floor: {
    name: "First floor", id: 1, rooms: [
        { id: 0, name: "Bedroom", icon: "weekend" },
        { id: 1, name: "Corridor", icon: "blur_linear" }
    ]
}});

// store.dispatch({type: 'FLUSH'});