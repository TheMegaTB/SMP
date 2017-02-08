import React from "react";
import DeviceControl from "./deviceControl";

export default class DeviceListItem extends React.Component {

    render() {
        let icon = "developer_board";

        if (this.props.device.type == "Fixture") icon = "lightbulb_outline";
        else if (this.props.device.type == "Shutter") icon = "call_to_action";

        return (
            <div className="mdl-list__item">
                <span className="mdl-list__item-primary-content">
                  <i className="material-icons grey icon">{icon}</i>
                  <span>{this.props.device.name}</span>
                </span>
                <DeviceControl device={this.props.device} />
            </div>
        );
    }
}

DeviceListItem.contextTypes = {
    store: React.PropTypes.object
};