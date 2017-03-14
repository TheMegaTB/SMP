import React from "react";
import {Slider, Toggle} from "material-ui";
import {ListItem} from "material-ui/List";

import {Socket} from "../websocket";
import {SLIDER_SMOOTHING_MS} from "../settings";

const sliderStyle = {marginTop: 0, marginBottom: 0};

export default class Device extends React.Component {
    constructor(props) {
        super(props);
        this.state = {value: 0};

        this.handleSlider = this.handleSlider.bind(this);
        this.handleToggle = this.handleToggle.bind(this);
        this.handleSliderRelease = this.handleSliderRelease.bind(this);
    }

    handleSlider(e, value) {
        this.setState({
            value: parseInt(value),
            lastSlideTarget: parseInt(value)
        });

        const device = this;
        if (!device.timeout) // TODO: Only use timeout when the user is still holding onto the slider else do it instantly
            device.timeout = setTimeout(() => {
                device.timeout = undefined;
                Socket.send(JSON.stringify({
                    action: "write",
                    channel: device.props.device.channel,
                    payload: device.state.lastSlideTarget
                }));
            }, SLIDER_SMOOTHING_MS);
    };

    handleSliderRelease() {
        if (this.timeout) clearTimeout(this.timeout);
        Socket.send(JSON.stringify({
            action: "write",
            channel: this.props.device.channel,
            payload: this.state.lastSlideTarget
        }));
    }

    handleToggle(e, value) {
        this.setState({
            value: value
        });

        Socket.send(JSON.stringify({
            action: "write",
            channel: this.props.device.channel,
            payload: value
        }));
    }

    shouldComponentUpdate(nextProps, nextState) {
        return (nextProps.device.value !== this.state.value) || (nextState.value !== this.state.value);
    }

    componentWillReceiveProps(nextProps) {
        if (this.state.value !== nextProps.device.value) {
            console.log("Update (" + this.props.device.name + ") -> " + nextProps.device.value, "from", this.state.value);
            this.setState({
                value: nextProps.device.value
            });
        }
    }

    componentWillMount() {
        this.setState({
            value: this.props.device.value
        });
    }

    render() {
        const properties = {
            primaryText: this.props.device.name
        };

        const attributes = this.props.device.attributes;
        if (attributes.indexOf("dimmable") > -1) {
            properties.rightToggle = <Slider
                min={0} max={255}
                value={this.state.value}
                sliderStyle={sliderStyle}
                onChange={this.handleSlider}
                onMouseUp={this.handleSliderRelease}
            />;

            //// This confuses the slider ... TODO search for a fix
            // const device = this;
            // properties.onClick = (e) => {
            //     // Set slider to max/min depending on current state
            //     console.log("Click!", e.target);
            //     if (device.state.value === 0) device.setState({value: 255});
            //     else if (device.state.value > 0) device.setState({value: 0});
            //     device.sendUpdate();
            // };
        } else if (attributes.indexOf("binary") > -1) {
            properties.rightToggle = <Toggle onToggle={this.handleToggle} toggled={this.state.value}/>;
        } else {
            // TODO: Do smth meaningful here
        }

        if (this.props.icon) properties.leftIcon = this.props.icon;
        else properties.insetChildren = true;

        return React.createElement(ListItem, properties);
    }
}