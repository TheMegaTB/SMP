import "webpack-material-design-icons/material-design-icons.css";
import "../css/material.min.css";
import "../css/popup.css";
import "../css/fonts/web fonts/roboto_regular_macroman/stylesheet.css";
import React from "react";
import {render} from "react-dom";
import Main from "./popup/components/main";
import {Provider} from "react-redux";
import {store} from "./popup/redux";

const renderApp = _ => {
    render(
        <Provider store={store}>
            <Main values={store.getState()}/>
        </Provider>,
        document.getElementById('app-container')
    )
};

store.subscribe(renderApp);
renderApp();