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

pub fn new_listener() -> mpsc::Sender<([u8; 4], SocketAddr)> {

    setup_gpio(24);

    let (tx, rx) = mpsc::channel();
    thread::Builder::new().name("plugin_gpio".to_string()).spawn(move || {
        for (data, _) in rx.iter() {
            let d: [u8; 4] = data;
            let (id, value): (&[u8], &[u8]) = d.split_at(2);
            let value = value[0];

            if id == [0, 0, 0] {
                write(24, value);
            } else { continue }
        }
    }).unwrap();
    tx
}
