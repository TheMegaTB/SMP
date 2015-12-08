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

pub fn new_listener(sock_tx: mpsc::Sender<([u8; 4], SocketAddr)>) -> mpsc::Sender<([u8; 4], SocketAddr)> {
    let (tx, rx) = mpsc::channel();
    thread::Builder::new().name("plugin_knx".to_string()).spawn(move || {
        for (data, src) in rx.iter() {
            let mut d: [u8; 4] = data;
            if d[0] == 0 && d[1] == 0 && d[2] == 1 {
                write(1, 2, 0, d[3]);
            } else if d[0] == 0 && d[1] == 0 && d[2] == 2 {
                write(0, 4, 0, d[3]);
            } else { continue }
            d.reverse();
            sock_tx.send((d, src)).unwrap();
        }
    }).unwrap();
    tx
}
