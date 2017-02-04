import React from "react";

export default class Fixture extends React.Component {
    render() {
        console.log("Device", this.props.device);
        return (
            <li className="mdl-list__item">
                <span className="mdl-list__item-primary-content">
                    <i className="icon material-icons"> wb_incandescend </i>
                    {this.props.device.name}
                </span>
                <span className="mdl-list__item-secondary-action">
                    <p style={{width: "300px"}}>
                        <input className="mdl-slider mdl-js-slider" type="range" min="0" max="100" tabIndex="0"/>
                    </p>
                </span>
            </li>
        );
    }
};