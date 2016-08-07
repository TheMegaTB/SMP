extern crate shared_objects;

use shared_objects::{ PluginType, DID, DeviceType, Return, Action };

#[no_mangle]
pub fn get_type(res: Return<PluginType>) {
    res.return_val(PluginType::Actor)
}

#[no_mangle]
pub fn get_responsibility(res: Return<DID>) {
    res.send_all(vec![
        DID {
            room: 0,
            dtype: DeviceType::Light,
            id: 0
        },
        DID {
            room: 1,
            dtype: DeviceType::Light,
            id: 0
        }
    ]);
}

#[no_mangle]
pub fn execute(action: Action) {
    println!("KNX: Executing {:?}", action);
}
