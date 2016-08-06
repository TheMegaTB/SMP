#![allow(dead_code)]
use libloading::os::unix::{Library, Symbol};

const LIB_PREFIX: &'static str = "lib";
#[cfg(target_os="macos")]
const LIB_POSTFIX: &'static str = ".dylib";
#[cfg(target_os="linux")]
const LIB_POSTFIX: &'static str = ".so";

pub struct PluginHandler {
    search_path: String,
    plugins: Vec<(&'static str, Library)>
}

impl PluginHandler {
    pub fn new(search_path: &'static str) -> PluginHandler {
        PluginHandler {
            search_path: search_path.to_string(),
            plugins: Vec::new()
        }
    }

    pub fn load(&mut self, name: &'static str) -> bool {
        match Library::new(self.search_path.clone() + "/" + &LIB_PREFIX.to_string() + &name + LIB_POSTFIX) {
            Ok(plugin) => {
                self.plugins.push((name, plugin));
                true
            },
            Err(err) => {
                println!("Error while loading plugin: {:?}", err);
                false
            }
        }
    }

    pub fn get_symbol<T>(&self, name: &'static str, symbol_name: &str) -> Option<Symbol<T>> {
        unsafe {
            let object = self.plugins.iter().find(|r| r.0 == name);
            match object {
                Some(plugin) => {
                    match plugin.1.get(symbol_name.as_bytes()) {
                        Ok(symbol) => Some(symbol),
                        _ => None
                    }
                },
                _ => None
            }
        }
    }

}
