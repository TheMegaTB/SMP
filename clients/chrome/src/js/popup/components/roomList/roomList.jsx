import React from "react";
import RoomListItem from "./roomListItem";

export default class RoomList extends React.Component {
    render() {
        return (
            <div className="mdl-list" style={{padding: "0 16px"}}>
                {this.props.rooms.map((room) => (
                        <RoomListItem key={room.name} room={room} floorID={this.props.floorID} />
                ))}
            </div>
        );
    }
}