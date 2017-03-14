import React from "react";

export default class Card extends React.Component {
    render() {
        return (
            <div className="mdl-shadow--4dp mdl-card" style={{width: "85%", margin: "20px auto", minHeight: "auto"}}>
                <div className="mdl-card__title">
                    <h2 className="mdl-card__title-text">{this.props.title}</h2>
                </div>
                <div className="mdl-card__actions mdl-card--border">
                    {this.props.content}
                </div>
            </div>
        );
    }
}