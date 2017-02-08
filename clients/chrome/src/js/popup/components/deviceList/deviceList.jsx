import React from "react";
import DeviceListItem from "./deviceListItem";

export default class DeviceList extends React.Component {
    render() {
        return (
            <div className="mdl-list" style={{padding: "0 16px"}}>
                {this.props.devices.map((device) => (
                        <DeviceListItem key={device.name} device={device} />
                ))}
            </div>
        );
    }
}