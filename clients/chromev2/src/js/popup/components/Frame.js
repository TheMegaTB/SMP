import React from "react";
import MoreVertIcon from "material-ui/svg-icons/navigation/more-vert";
import NavigationClose from "material-ui/svg-icons/navigation/close";
import {Drawer, MenuItem, AppBar, IconButton, IconMenu, Subheader, Divider} from "material-ui";
import {Scrollbars} from "react-custom-scrollbars";
import {FLOOR_NAMES, ROOM_NAMES} from "../settings";

const BarMenu = (props) => (
    <IconMenu
        iconButtonElement={
            <IconButton><MoreVertIcon /></IconButton>
        }
        targetOrigin={{horizontal: 'right', vertical: 'top'}}
        anchorOrigin={{horizontal: 'right', vertical: 'top'}}
    >
        <MenuItem primaryText="Refresh"/>
        <MenuItem primaryText="Help"/>
        <MenuItem primaryText="Sign out"/>
    </IconMenu>
);

function getRoomName(floor, room) {
    if (ROOM_NAMES[floor] && ROOM_NAMES[floor][room])
        return ROOM_NAMES[floor][room];
    return floor + "-" + room;
}

export default class Frame extends React.Component {

    constructor(props) {
        super(props);
        this.state = {open: false};

        this.handleToggle = this.handleToggle.bind(this);
        this.handleClose = this.handleClose.bind(this);
    }

    handleToggle() {
        this.setState({open: !this.state.open});
    }

    handleClose(floor, room) {
        if (floor && room)
            this.context.store.dispatch({type: 'FILTER', filter: [floor, room, /.*/]});
        this.setState({open: false});
    }

    render() {
        const state = this.context.store.getState();

        const floors = {};
        for (let dev in state.devices) {
            if (!state.devices.hasOwnProperty(dev)) continue;
            dev = state.devices[dev];

            const floor = dev.channel[0];
            const room = dev.channel[1];
            if (!floors[floor]) floors[floor] = [room];
            else if (floors[floor].indexOf(room) < 0) floors[floor].push(room);
        }

        const drawerItems = [];
        let first = true;
        let rooms;
        for (let floor in floors) {
            if (!floors.hasOwnProperty(floor)) continue;
            rooms = floors[floor];

            if (rooms) {
                drawerItems.push(<Divider key={floor + "_div"}/>);
            } else first = false;

            drawerItems.push(<Subheader
                key={floor + "_head"}>{FLOOR_NAMES[floor] ? FLOOR_NAMES[floor] : floor}</Subheader>);
            for (let room in rooms) {
                if (!rooms.hasOwnProperty(room)) continue;
                const roomName = getRoomName(floor, rooms[room]);
                drawerItems.push(<MenuItem onTouchTap={this.handleClose.bind(this, floor, rooms[room])}
                                           key={floor + rooms[room]}>{roomName}</MenuItem>);
            }
        }

        return (
            <div>
                <AppBar
                    title={getRoomName(state.filter[0].toString(), state.filter[1].toString())}
                    iconElementRight={<BarMenu/>}
                    onLeftIconButtonTouchTap={this.handleToggle}
                />
                <Drawer
                    docked={false}
                    width={200}
                    open={this.state.open}
                    onRequestChange={(open) => this.setState({open})}
                >
                    <AppBar title="Rooms" iconElementLeft={<IconButton><NavigationClose /></IconButton>}
                            onLeftIconButtonTouchTap={this.handleClose}/>
                    <Scrollbars
                        style={{height: "calc(100% - 64px)"}}>
                        {drawerItems}
                    </Scrollbars>
                </Drawer>
            </div>
        );
    }
}

Frame.contextTypes = {
    store: React.PropTypes.object
};