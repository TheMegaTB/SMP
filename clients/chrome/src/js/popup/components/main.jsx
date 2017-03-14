import React from "react";
import Header from "./header";
import Drawer from "./drawer";
import Page from "./page";
import Card from "./card";
import RoomList from "./roomList/roomList";
import DeviceList from "./deviceList/deviceList";
import {connect} from "react-redux";
import {channelToID} from "../redux";

let deviceCount = 0;
let currentPage = undefined;

const ch = require('exports?componentHandler!material-design-lite/material.js');
export default class Main extends React.Component {
    // componentDidMount() {
    //     // Make those MDL things work...
    //     ch.upgradeDom();
    // }

    shouldComponentUpdate(nextProps, nextState, nextContext) {
        // console.log("HI");
        // console.log("NEXT CONTEXT:", nextContext);
        // console.log(this.props.values.target != nextProps.values.target);
        // console.log(this.props.values.target, nextProps.values.target);
        // return this.props.values.target != nextProps.values.target;
        console.log(currentPage, nextContext.store.getState().target);
        return (deviceCount !== Object.keys(nextContext.store.getState().devices).length)
            || currentPage !== nextContext.store.getState().target;
    }

    render() {
        console.log("MAIN RERENDER", this.props.target, this.props.state);
        const state = this.context.store.getState();
        const floors = state.floors;
        const target = state.target;

        deviceCount = Object.keys(state.devices).length;
        currentPage = state.target;

        let cards = [];

        switch (target.length) {
            case 0:
                for (let floor in floors) {
                    if (!floors.hasOwnProperty(floor)) continue;
                    floor = floors[floor];
                    cards.push(
                        <Card key={floor.name} title={floor.name} content={<RoomList rooms={floor.rooms} floorID={floor.id}/>}/>
                    )
                }
                break;
            case 2:
                // Push device cards
                const devices = {};
                outerLoop:
                    for (let device in state.devices) {
                        if (!state.devices.hasOwnProperty(device)) continue;
                        device = state.devices[device];
                        for (let entry in target) {
                            if (!target.hasOwnProperty(entry)) continue;
                            if (device.channel[entry] !== target[entry]) continue outerLoop;
                        }
                        if (devices[device.type] === undefined) devices[device.type] = [channelToID(device.channel)];
                        else devices[device.type].push(channelToID(device.channel));
                    }

                for (let type in devices) {
                    if (!devices.hasOwnProperty(type)) continue;
                    cards.push(
                        <Card key={type} title={type} content={<DeviceList devices={devices[type]} />}/>
                    )
                }
                break;
        }

        return (
            <div className="mdl-layout mdl-js-layout mdl-layout--fixed-header">
                <Header title="Main menu"/>
                <Drawer/>
                <Page content={cards} />
            </div>
        );
    }
}

Main.contextTypes = {
    store: React.PropTypes.object
};