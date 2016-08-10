extern crate shared_objects;

use std::process::Command;
use std::path::PathBuf;

use shared_objects::{ PluginType, DID, DeviceType, Return, Action };

const GROUPS_WRITE_PATH: &'static str = "/usr/local/bin/groupswrite";

const MAPPINGS: [(DID, [u8; 3]); 2] = [
    (DID {
        room: 0,
        dtype: DeviceType::Light,
        id: 0
    }, [0, 4, 0]),
    (DID {
        room: 0,
        dtype: DeviceType::Light,
        id: 1
    }, [1, 2, 0]),
];

fn write(group: u8, subgroup: u8, id: u8, value: u8) {
    let id = format!("{}/{}/{}", group, subgroup, id);
    Command::new("/usr/local/bin/groupswrite").arg("ip:localhost").arg(id).arg(value.to_string()).output().unwrap_or_else(|e| {
        panic!("failed to execute process: {}", e)
    });
}

#[no_mangle]
pub fn get_type(res: Return<PluginType>) {
    if PathBuf::from(GROUPS_WRITE_PATH).metadata().is_ok() {
        res.return_val(PluginType::Actor);
    } // else this plugin is useless 'cause eibd is not installed
}

#[no_mangle]
pub fn get_responsibility(res: Return<DID>) {
    for &(ref did, _) in MAPPINGS.iter() {
        res.send(did.clone());
    }
}

#[no_mangle]
pub fn execute(action: Action) {
    println!("KNX: Executing {:?}", action);
    for &(ref did, knx_id) in MAPPINGS.iter() {
        if *did == action.target {
            write(knx_id[0], knx_id[1], knx_id[2], (action.change.clone().to_value() > 0) as u8)
        }
    }
}
