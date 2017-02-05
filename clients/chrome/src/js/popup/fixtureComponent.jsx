import React from "react";
import FixtureControl from "./fixtureControlComponent";

// const ch = require('exports?componentHandler!material-design-lite/material.js');
export default class Fixture extends React.Component {
    // componentDidMount() {
    //     // Make those MDL sliders work...
    //     ch.upgradeDom();
    // }

    render() {

        let icon = "developer_board";

        if (this.props.device.type == "Fixture") icon = "lightbulb_outline";
        else if (this.props.device.type == "Shutter") icon = "call_to_action";



        return (
            <li className="mdl-list__item">
                <span className="mdl-list__item-primary-content">
                    <i className="icon grey material-icons">{icon}</i>
                    {this.props.device.name}
                </span>
                <FixtureControl device={this.props.device}/>
            </li>
        );
    }
};