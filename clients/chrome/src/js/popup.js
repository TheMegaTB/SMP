import "webpack-material-design-icons/material-design-icons.css";
import "../css/material.min.css";
import "../css/popup.css";
import React from "react";
import {render} from "react-dom";
import Main from "./popup/components/main";

render(
    <Main/>,
    window.document.getElementById("app-container")
);
