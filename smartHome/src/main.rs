extern crate libloading;
extern crate glob;
extern crate shared_objects;

mod plugin_handler;
use plugin_handler::PluginHandler;

use shared_objects::PluginType;

use std::sync::mpsc;

const PLUGIN_PATH: &'static str = "plugins";

fn main() {
    println!("Hello world!");

    let mut plugins = PluginHandler::new(PLUGIN_PATH);

    let plugin = "knx";

    plugins.load(plugin);

    match plugins.get_symbol::<fn() -> PluginType>(plugin, "init") {
        Some(init_fn) => {
            println!("init");
            println!("{:?}", init_fn());
        },
        None => {}
    }
}
