import React from "react";

export default class Header extends React.Component {
    render() {
        return (
            <div className="mdl-layout__drawer">
                <span className="mdl-layout-title">Rooms</span>
                <nav className="mdl-navigation">
                    <a className="mdl-navigation__link" href="">Room 1</a>
                    <a className="mdl-navigation__link" href="">Room 2</a>
                    <a className="mdl-navigation__link" href="">Room 3</a>
                    <a className="mdl-navigation__link" href="">Room 4</a>
                </nav>
            </div>
        );
    }
}