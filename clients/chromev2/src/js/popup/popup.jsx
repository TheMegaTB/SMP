import "webpack-material-design-icons/material-design-icons.css";
import "../../css/popup.css";
import "../../css/fonts/web fonts/roboto_regular_macroman/stylesheet.css";

import React from "react";
import {render} from "react-dom";
import {Provider} from "react-redux";
import {store} from "./redux";

import MuiThemeProvider from "material-ui/styles/MuiThemeProvider";

import AppBar from "material-ui/AppBar";
import Room from "./components/Room";
import {Snackbar} from "material-ui";

var injectTapEventPlugin = require("react-tap-event-plugin");
injectTapEventPlugin();

const App = () => (
    <MuiThemeProvider>
        <div>
            <AppBar
                title="Bedroom"
                iconClassNameRight="muidocs-icon-navigation-expand-more"
            />
            <Room devices={store.getState().devices}/>
            {/*<Snackbar*/}
            {/*open={true}*/}
            {/*message="Refreshing devices ..."*/}
            {/*autoHideDuration={2000}*/}
            {/*/>*/}
        </div>
    </MuiThemeProvider>
);

const renderApp = _ => {
    render(
        <Provider store={store}>
            <App/>
        </Provider>,
        document.getElementById('app-container')
    )
};

store.subscribe(renderApp);
renderApp();