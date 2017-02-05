import "webpack-material-design-icons/material-design-icons.css";
import "../css/material.min.css";
import "../css/popup.css";
import Room from "./popup/roomComponent";
import React from "react";
import {render} from "react-dom";

render(
    <Room />,
    window.document.getElementById("app-container")
);
