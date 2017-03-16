import "webpack-material-design-icons/material-design-icons.css";
import "../../css/popup.css";
import "../../css/fonts/web fonts/roboto_regular_macroman/stylesheet.css";
import React from "react";
import {render} from "react-dom";
import {Provider} from "react-redux";
import {store} from "./redux";
import MuiThemeProvider from "material-ui/styles/MuiThemeProvider";
import Room from "./components/Room";
import Frame from "./components/Frame";
import {Snackbar} from "material-ui";

var injectTapEventPlugin = require("react-tap-event-plugin");
injectTapEventPlugin();

function filterByRegex(devices, regex) {
    const res = {};
    for (let device in devices) {
        if (!devices.hasOwnProperty(device)) continue;
        const channel = devices[device].channel;
        if (channel[0].toString().match(regex[0]) && channel[1].toString().match(regex[1]) && channel[2].toString().match(regex[2]))
            res[device] = devices[device];
    }
    return res;
}

class App extends React.Component {
    render() {
        const state = store.getState();
        const devices = filterByRegex(state.devices, state.filter);
        return (
            <MuiThemeProvider>
                <div>
                    <Frame/>
                    {/*TODO: Filter by current room*/}
                    <Room devices={devices}/>
                    {/*<Snackbar*/}
                    {/*open={true}*/}
                    {/*message="Refreshing devices ..."*/}
                    {/*autoHideDuration={2000}*/}
                    {/*/>*/}
                </div>
            </MuiThemeProvider>
        );
    }
}

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