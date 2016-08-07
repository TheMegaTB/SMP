extern crate libloading;
extern crate glob;
extern crate shared_objects;

mod plugin_handler;
use plugin_handler::PluginHandler;

use shared_objects::PluginType;

// use std::sync::mpsc;

const PLUGIN_PATH: &'static str = "plugins";

fn main() {
    println!("Hello world!");

    let plugins = PluginHandler::new(PLUGIN_PATH);

    for &(ref name, _) in plugins.plugins.iter() {
        match plugins.get_symbol::<fn() -> PluginType>(name, "get_type") {
            Some(init_fn) => {
                println!("INIT {}:{:?}", name, init_fn());
            },
            None => {
                println!("ERROR: No get_type method provided for plugin '{}'", name);
            }
        }
    }
}
