import React from "react";
import Fixture from "./fixtureComponent";
import {Socket} from "../websocket";

export default class Room extends React.Component {

    updateDevices() {
        this.setState({
            devices: []
        });
        this.ws.send(JSON.stringify({action: "query"}));
    }

    constructor(props) {
        super(props);
        this.state = {
            devices: []
        };

        this.ws = Socket;

        const room = this;

        this.ws.onopen = function (event) {
            console.log("Connection established!");
            room.updateDevices();
        };

        this.ws.onmessage = function (event) {
            const data = JSON.parse(event.data);

            if (data.action == "announce") {
                room.setState({
                    devices: room.state.devices.concat([data])
                });
            }
        };
    }

    render() {
        const fixtures = this.state.devices.map((device) =>
            <Fixture device={device} key={device.name}/>
        );

        return (
            <div className="mdl-layout mdl-js-layout mdl-layout--fixed-header">
                <header className="mdl-layout__header">
                    <div className="mdl-layout__header-row">
                        <span className="mdl-layout-title">Room 1</span>
                        <div className="mdl-layout-spacer"></div>
                    </div>
                </header>
                <div className="mdl-layout__drawer">
                    <span className="mdl-layout-title">Rooms</span>
                    <nav className="mdl-navigation">
                        <a className="mdl-navigation__link" href="">Room 1</a>
                        <a className="mdl-navigation__link" href="">Room 2</a>
                        <a className="mdl-navigation__link" href="">Room 3</a>
                        <a className="mdl-navigation__link" href="">Room 4</a>
                    </nav>
                </div>
                <main className="mdl-layout__content">
                    <div className="page-content">
                        <ul className="mdl-list" id="fixtures">
                            {fixtures}
                        </ul>
                    </div>
                </main>
            </div>
        );
    }
};