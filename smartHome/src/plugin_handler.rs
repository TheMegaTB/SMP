#![allow(dead_code)]
use libloading::os::unix::{Library, Symbol};
use std::path::PathBuf;
use std::fs;

const LIB_PREFIX: &'static str = "lib";
#[cfg(target_os="macos")]
const LIB_POSTFIX: &'static str = ".dylib";
#[cfg(target_os="linux")]
const LIB_POSTFIX: &'static str = ".so";

fn get_plugin_name(path: &PathBuf) -> String {
    path.file_name().unwrap().to_str().unwrap().to_string().replace(LIB_PREFIX, "").replace(LIB_POSTFIX, "")
}

pub struct PluginHandler {
    search_path: PathBuf,
    pub plugins: Vec<(String, Library)>
}

impl PluginHandler {
    pub fn new(search_path: &'static str) -> PluginHandler {
        let mut handler = PluginHandler {
            search_path: PathBuf::from(search_path),
            plugins: Vec::new()
        };
        handler.search();
        handler
    }

    pub fn search(&mut self) {
        let paths = fs::read_dir(&self.search_path).unwrap();

        for path in paths {
            self.load(path.unwrap().path());
        }
    }

    pub fn load(&mut self, path: PathBuf) -> bool {
        let name = get_plugin_name(&path);
        match Library::new(path.clone()) {
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

    pub fn get_symbol<T>(&self, name: &str, symbol_name: &str) -> Option<Symbol<T>> {
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
