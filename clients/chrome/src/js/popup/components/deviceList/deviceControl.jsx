import React from "react";
import {Socket} from "../../../websocket";
import {SLIDER_SMOOTHING_MS} from "../../../config";

const ch = require('exports?componentHandler!material-design-lite/material.js');
export default class DeviceControl extends React.Component {
    constructor(props) {
        super(props);
        this.state = {value: 0};

        // This binding is necessary to make `this` work in the callback
        this.handleSlide = this.handleSlide.bind(this);
        this.handleBinary = this.handleBinary.bind(this);
        // this.handleShutter = this.handleShutter.bind(this);
    }

    componentDidMount() {
        // Make those MDL sliders work...
        ch.upgradeDom();
    }

    handleSlide(e) {
        e.preventDefault();
        this.setState({
            value: e.target.value
        });

        const control = this;
        if (!control.timeout)
            control.timeout = setTimeout(() => {
                control.timeout = undefined;
                Socket.send(JSON.stringify({
                    action: "write",
                    channel: control.props.device.channel,
                    payload: parseInt(control.state.value)
                }));
            }, SLIDER_SMOOTHING_MS);
    }

    handleBinary() {
        const newValue = !this.state.value;
        this.setState({
            value: newValue
        });
        Socket.send(JSON.stringify({
            action: "write",
            channel: this.props.device.channel,
            payload: newValue
        }));
    }

    handleShutter(action) {
        Socket.send(JSON.stringify({
            action: "write",
            channel: this.props.device.channel,
            payload: action
        }));
    }

    render() {
        let control = (
            <div>No controls available</div>
        );

        let containerStyle = {width: "50%"};

        // TODO Go by attributes rather than types
        if (this.props.device.attributes.indexOf("dimmable") > -1) {
            control = (
                <p>
                    <input onChange={this.handleSlide} className="mdl-slider mdl-js-slider" type="range" min="0"
                           max="100" tabIndex="0"/>
                </p>
            );
        } else if (this.props.device.attributes.indexOf("binary") > -1) {
            containerStyle = {};
            control = (
                <div style={{margin: "0 20px"}}>
                    <label className="mdl-switch mdl-js-switch mdl-js-ripple-effect" htmlFor={this.props.device.name}>
                        <input type="checkbox" id={this.props.device.name} className="mdl-switch__input" checked={this.state.value}
                               onChange={this.handleBinary}/>
                        <span className="mdl-switch__label"/>
                    </label>
                </div>
            )
        } else if (this.props.device.type == "Shutter") {
            control = (
                <div style={{display: "flex", justifyContent: "space-between", margin: "0 20px"}}>
                    <button className="mdl-button mdl-js-button mdl-button--icon"
                            onClick={this.handleShutter.bind(this, "down")}>
                        <i className="grey material-icons">arrow_downward</i>
                    </button>
                    <button className="mdl-button mdl-js-button mdl-button--icon"
                            onClick={this.handleShutter.bind(this, "stop")}>
                        <i className="grey material-icons">cancel</i>
                    </button>
                    <button className="mdl-button mdl-js-button mdl-button--icon"
                            onClick={this.handleShutter.bind(this, "up")}>
                        <i className="grey material-icons">arrow_upward</i>
                    </button>
                </div>
            );
        }

        return (
            <span className="mdl-list__item-secondary-action" style={containerStyle}>
                {control}
            </span>
        );
    }
};