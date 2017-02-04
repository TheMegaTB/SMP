import React from "react";
import Fixture from "./fixtureComponent";


// export const devices = [];

// function send(ws, data) {
//     ws.send(JSON.stringify(data));
// }

// function updateDevices() {
//     devices.length = 0; // Clear all devices
//     send({action: "query"});
// }

// ws.onmessage = function (event) {
//     const data = JSON.parse(event.data);
//
//     if (data.action == "announce") {
//         devices.push(data);
//     }
//
//     // console.log("MSG:", JSON.parse(event.data));
// };

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

        this.ws = new WebSocket("ws://127.0.0.1:3003/");

        const room = this;

        this.ws.onopen = function (event) {
            console.log("Connection established!");
            room.updateDevices();
        };

        this.ws.onmessage = function (event) {
            const data = JSON.parse(event.data);

            if (data.action == "announce") {
                console.log("Pushing device");
                room.setState({
                    devices: room.state.devices.concat([data])
                });
                // room.state.devices.push(data);
                console.log(room.state.devices);
            }
        };
    }

    render() {
        const fixtures = this.state.devices.map((device) =>
            <Fixture device={device} key={device.name}/>
        );

        console.log("Fixtures:", fixtures);

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