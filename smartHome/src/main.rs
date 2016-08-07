extern crate libloading;
extern crate glob;
extern crate shared_objects;

use std::collections::HashMap;
use std::sync::mpsc;

mod plugin_handler;
use plugin_handler::PluginHandler;

use shared_objects::{ PluginType, DID, Return, ReturnValue, Action };

const PLUGIN_PATH: &'static str = "plugins";

fn main() {
    println!("Hello world!");

    let plugin_handler = PluginHandler::new(PLUGIN_PATH);

    let mut inputs = Vec::new();
    let mut actors: HashMap<DID, Vec<String>> = HashMap::new();
    let mut wildcard_actors = Vec::new();
    for &(ref name, _) in plugin_handler.plugins.iter() {
        let (tx, rx) = ReturnValue::new();
        plugin_handler.get_symbol_or_fail::<fn(Return<PluginType>)>(name, "get_type")(tx);
        let plugin_types = rx.receive_all();
        println!("INIT {}:{:?}", name, plugin_types);
        for plugin_type in plugin_types.into_iter() {
            match plugin_type {
                PluginType::Actor => {
                    let (tx, rx) = ReturnValue::new();
                    plugin_handler.get_symbol_or_fail::<fn(Return<DID>)>(name, "get_responsibility")(tx);
                    let res = rx.receive_all();
                    if res.len() > 0 {
                        for did in res.into_iter() {
                            if !match actors.get_mut(&did) {
                                Some(vec) => { vec.push(name.clone()); true },
                                None => false
                            } {
                                actors.insert(did, vec![name.clone()]);
                            }
                        }
                    } else {
                        wildcard_actors.push(name.clone());
                    }
                },
                PluginType::Input => {
                    let (tx, rx) = ReturnValue::new();
                    plugin_handler.get_symbol_or_fail::<fn(Return<Action>)>(name, "listen")(tx);
                    inputs.push(rx);
                }
            }
        }
    }

    loop {
        let mut dead_plugins = Vec::new();
        for (id, rx) in inputs.iter().enumerate() {
            match rx.receive() {
                Ok(action) => {
                    match actors.get(&action.target) {
                        Some(plugins) => {
                            for plugin in plugins.iter().chain(wildcard_actors.iter()) {
                                println!("Calling '{}'", plugin);
                                plugin_handler.get_symbol_or_fail::<fn(Action)>(plugin, "execute")(action.clone());
                            }
                        },
                        None => {}
                    }
                },
                Err(e) => {
                    match e {
                        mpsc::TryRecvError::Disconnected => dead_plugins.push(id),
                        _ => {}
                    }
                }
            }
        }
        dead_plugins.reverse();
        for dead_plugin in dead_plugins.into_iter() {
            println!("WARN: A input plugin just died");
            inputs.remove(dead_plugin);
        }
    }
}
