import React from "react";
import ListItem from "./listItem";

export default class List extends React.Component {
    render() {
        return (
            <div className="mdl-list" style={{padding: "0 16px"}}>
                {this.props.rooms.map((room) => (
                        <ListItem key={room.name} title={room.name} icon={room.icon} />
                ))}
            </div>
        );
    }
}