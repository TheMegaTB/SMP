import React from "react";
import DeviceControl from "./deviceControl";

export default class DeviceListItem extends React.Component {

    shouldComponentUpdate(nextProps, nextState) {
        console.log("name", this.props.device.name === nextProps.device.name);
        return this.props.device.name === nextProps.device.name;
    }

    render() {
        let icon = "developer_board";

        const device = this.context.store.getState().devices[this.props.device];

        if (device.type === "Fixture") icon = "lightbulb_outline";
        else if (device.type === "Shutter") icon = "call_to_action";

        return (
            <div className="mdl-list__item">
                <span className="mdl-list__item-primary-content">
                  <i className="material-icons grey icon">{icon}</i>
                  <span>{device.name}</span>
                </span>
                <DeviceControl device={device}/>
            </div>
        );
    }
}

DeviceListItem.contextTypes = {
    store: React.PropTypes.object
};