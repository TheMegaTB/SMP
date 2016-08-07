extern crate shared_objects;

use shared_objects::{ PluginType };

#[no_mangle]
pub fn get_type() -> PluginType {
    return PluginType::Input;
}
