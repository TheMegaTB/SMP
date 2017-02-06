import "webpack-material-design-icons/material-design-icons.css";
import "../css/material.min.css";
import "../css/popup.css";
import React from "react";
import {render} from "react-dom";
import MainComponent from "./popup/components/main";
import {Provider} from "react-redux";
import {store} from "./popup/redux";

render(
    <Provider store={store}>
        <MainComponent/>
    </Provider>,
    window.document.getElementById("app-container")
);