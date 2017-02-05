import React from "react";

export default class ListItem extends React.Component {
    render() {
        return (
            <div className="mdl-list__item selectable-item">
                <span className="mdl-list__item-primary-content">
                  <i className="material-icons grey icon">{this.props.icon}</i>
                  <span>{this.props.title}</span>
                </span>
                <a className="mdl-list__item-secondary-action" href="#"><i className="material-icons">keyboard_arrow_right</i></a>
            </div>
        );
    }
}