import React from "react";
import Header from "./header";
import Drawer from "./drawer";
import Page from "./page";
import Card from "./card";
import List from "./list/list";

export default class Main extends React.Component {
    constructor(props) {
        super(props);
        this.state = {
            selected: undefined
        };
    }

    render() {
        const floors = [
            { name: "Ground floor", rooms: [
                { name: "Living room", icon: "weekend" },
                { name: "Corridor", icon: "blur_linear" }
            ]},
            { name: "Second floor", rooms: [
                { name: "Bedroom", icon: "weekend" },
                { name: "Corridor", icon: "blur_linear" }
            ]}
        ];

        const cards = floors.map((floor) => (
            <Card key={floor.name} title={floor.name} content={<List rooms={floor.rooms} />} />
        ));

        return (
            <div className="mdl-layout mdl-js-layout mdl-layout--fixed-header">
                <Header title="Main menu"/>
                <Drawer/>
                <Page content={cards} />
            </div>
        );
    }
}