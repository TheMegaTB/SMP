import React from "react";

export default class RoomListItem extends React.Component {
    constructor(props) {
        super(props);

        this.handleClick = this.handleClick.bind(this);
    }

    handleClick() {
        // console.log(this.props.room.id, this.props.floorID);
        this.context.store.dispatch({
            type: 'NAVIGATE',
            target: [this.props.floorID, this.props.room.id]
        });
    }

    render() {
        return (
            <div className="mdl-list__item selectable-item" onClick={this.handleClick}>
                <span className="mdl-list__item-primary-content">
                  <i className="material-icons grey icon">{this.props.room.icon}</i>
                  <span>{this.props.room.name}</span>
                </span>
                <a className="mdl-list__item-secondary-action" href="#"><i className="material-icons">keyboard_arrow_right</i></a>
            </div>
        );
    }
}

RoomListItem.contextTypes = {
    store: React.PropTypes.object
};