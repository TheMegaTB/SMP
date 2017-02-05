import React from "react";

export default class Card extends React.Component {
    render() {
        return (
            <div className="mdl-shadow--4dp mdl-card" style={{width: "85%", margin: "20px auto"}}>
                <div className="mdl-card__title" style={{padding: "16px 16px 0 16px"}}>
                    <h2 className="mdl-card__title-text">{this.props.title}</h2>
                </div>
                <div className="mdl-card__supporting-text">
                    {this.props.content}
                </div>
            </div>
        );
    }
}