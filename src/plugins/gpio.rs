use std::thread;
use std::sync::mpsc;
use std::net::SocketAddr;
use std::process::Command;

fn write(pin: u8, value: u8) {
    let value = if value == 0 { "1" } else { "0" };
    Command::new("/usr/local/bin/gpio").arg("-g").arg("write").arg(pin.to_string()).arg(value).status().unwrap_or_else(|e| {
        panic!("failed to execute process: {}", e)
    });
}

fn setup_gpio(pin: u8) {
    Command::new("/usr/local/bin/gpio").arg("-g").arg("mode").arg(pin.to_string()).arg("out").status().unwrap_or_else(|e| {
        panic!("failed to execute process: {}", e)
    });

    Command::new("/usr/local/bin/gpio").arg("-g").arg("write").arg(pin.to_string()).arg("1").status().unwrap_or_else(|e| {
        panic!("failed to execute process: {}", e)
    });
}

pub fn new_listener(cb: mpsc::Sender<([u8; 4], SocketAddr)>) -> mpsc::Sender<([u8; 4], SocketAddr)> {

    setup_gpio(24);

    let (tx, rx): (mpsc::Sender<([u8; 4], SocketAddr)>, _) = mpsc::channel();
    thread::Builder::new().name("plugin_gpio".to_string()).spawn(move || {
        for data in rx.iter() {
            let (id, value): (&[u8], &[u8]) = data.0.split_at(3);

            if id == [0, 0, 0] {
                write(24, value[0]);
                cb.send(data).unwrap();
            } else { continue }
        }
    }).unwrap();
    tx
}
