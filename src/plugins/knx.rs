use std::thread;
use std::sync::mpsc;
use std::net::SocketAddr;
use std::process::Command;

fn write(group: u8, subgroup: u8, id: u8, value: u8) {
    let id = format!("{}/{}/{}", group, subgroup, id);
    Command::new("/usr/local/bin/groupswrite").arg("ip:localhost").arg(id).arg(value.to_string()).output().unwrap_or_else(|e| {
        panic!("failed to execute process: {}", e)
    });
}

pub fn new_listener(cb: mpsc::Sender<([u8; 4], SocketAddr)>) -> mpsc::Sender<([u8; 4], SocketAddr)> {
    let (tx, rx): (mpsc::Sender<([u8; 4], SocketAddr)>, _) = mpsc::channel();
    thread::Builder::new().name("plugin_knx".to_string()).spawn(move || {
        for data in rx.iter() {
            let (id, value): (&[u8], &[u8]) = data.0.split_at(3);

            if id == [0, 0, 1] {
                write(1, 2, 0, value[0]);
                cb.send(data).unwrap();
            } else if id == [0, 0, 2] {
                write(0, 4, 0, value[0]);
                cb.send(data).unwrap();
            } else { continue }
        }
    }).unwrap();
    tx
}
