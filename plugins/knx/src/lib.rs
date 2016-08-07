extern crate shared_objects;

use shared_objects::{ PluginType, DID };

#[no_mangle]
pub fn get_type() -> PluginType {
    return PluginType::Actor;
}

#[no_mangle]
pub fn is_responsible_for(did: DID) -> bool {
    did == "RoomA/Light/0".to_string()
}
