import React from "react";
import {Divider, FontIcon, Slider, Subheader, Toggle} from "material-ui";
import {List, ListItem} from "material-ui/List";

import Lightbulb from "material-ui/svg-icons/action/lightbulb-outline";
import Shutter from "material-ui/svg-icons/av/web-asset";
import Scene from "material-ui/svg-icons/hardware/developer-board";
import DefaultIcon from "material-ui/svg-icons/navigation/apps";

import Device from "./Device";

const sliderStyle = {marginTop: 0, marginBottom: 0};

const deviceTypes = {
    "Fixture": {
        name: "Lights",
        icon: <Lightbulb/>
    },
    "Shutter": {
        name: "Shutters",
        icon: <Shutter/>
    },
    "Scene": {
        name: "Scenes",
        icon: <Scene/>
    }
};

export default class Room extends React.Component {
    render() {
        const categories = new Map();

        for (let device in this.props.devices) {
            if (!this.props.devices.hasOwnProperty(device)) continue;

            device = this.props.devices[device];

            if (categories.has(device.type)) categories.get(device.type).push(device);
            else categories.set(device.type, [device]);
        }

        let first = false;
        const deviceLists = [];
        categories.forEach((devices, category) => {
            if (!first) {
                deviceLists.push(<Divider key={"div_" + category}/>);
                first = false;
            }

            category = deviceTypes.hasOwnProperty(category) ? deviceTypes[category] : {
                name: category,
                icon: <DefaultIcon/>
            };

            deviceLists.push(
                <List key={category.name}>
                    <Subheader>{category.name}</Subheader>
                    {devices.map((device, i) => {
                        const properties = {
                            device: device,
                            key: device.name + i
                        };

                        if (i === 0) properties.icon = category.icon;

                        return React.createElement(Device, properties);
                    })}
                </List>
            );
        });

        return (
            <div>
                {deviceLists}
            </div>
        );

        return (
            <div>
                <List>
                    <Subheader>Lights</Subheader>
                    <ListItem
                        leftIcon={<Lightbulb/>}
                        primaryText="Ceiling light" rightToggle={<Toggle />}
                    />
                    <ListItem
                        insetChildren={true}
                        primaryText="Uplight"
                        rightToggle={<Slider defaultValue={0.5} sliderStyle={sliderStyle}/>}
                    />
                    <ListItem
                        insetChildren={true} primaryText="Bed lamp" rightToggle={<Toggle />}/>
                </List>
                <Divider/>
                <List>
                    <Subheader>Shutters</Subheader>
                    <ListItem
                        leftIcon={<Shutter/>}
                        primaryText="Window" rightToggle={<Toggle />}
                    />
                    <ListItem
                        insetChildren={true} primaryText="Door" rightToggle={<Toggle />}/>
                </List>
                <Divider/>
                <List>
                    <Subheader>Scenes</Subheader>
                    <Device first={true}/>
                    <Device/>
                </List>
            </div>
        );
    }
}