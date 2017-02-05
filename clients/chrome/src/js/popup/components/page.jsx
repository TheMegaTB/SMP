import React from "react";

export default class Page extends React.Component {
    render() {
        return (
            <main className="mdl-layout__content">
                <div className="page-content">
                    {this.props.content}
                </div>
            </main>
        );
    }
}