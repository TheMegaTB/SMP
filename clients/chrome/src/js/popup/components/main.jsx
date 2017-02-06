import React from "react";
import Header from "./header";
import Drawer from "./drawer";
import Page from "./page";
import Card from "./card";
import RoomList from "./roomList/roomList";
import {connect} from "react-redux";

class Main extends React.Component {
    render() {
        console.log("MAIN RERENDER");
        const {store} = this.context;
        const state = store.getState();
        const floors = state.floors;
        const target = state.target;

        const cards = [];

        switch (target.length) {
            case 0:
                for (let floor in floors) {
                    if (!floors.hasOwnProperty(floor)) continue;
                    floor = floors[floor];
                    cards.push(
                        <Card key={floor.name} title={floor.name} content={<RoomList rooms={floor.rooms} floorID={floor.id}/>}/>
                    )
                }
                break;
            case 1:
                // Push device cards
                break;
        }

        return (
            <div className="mdl-layout mdl-js-layout mdl-layout--fixed-header">
                <Header title="Main menu"/>
                <Drawer/>
                <Page content={cards} />
            </div>
        );
    }
}

Main.contextTypes = {
    store: React.PropTypes.object
};

// function mapStateToProps(state) {
//     console.log(state.target);
//     return { target: state.target }
// }
//
// const MainComponent = connect(mapStateToProps)(Main);
// export default MainComponent;
export default connect(state => state)(Main)