extern crate shared_objects;

use shared_objects::{ PluginType, Return, Action, StateValue, State, DeviceType, DID };

#[no_mangle]
pub fn get_type(res: Return<PluginType>) {
    res.return_val(PluginType::Input)
}

#[no_mangle]
pub fn listen(res: Return<Action>) {
    std::thread::spawn(move || {
        std::thread::sleep(std::time::Duration::from_secs(2));
        res.send(Action {
            change: State::Automatic(StateValue::Value(255)),
            target: DID {
                room: 0,
                dtype: DeviceType::Light,
                id: 0
            }
        });
    });
}
