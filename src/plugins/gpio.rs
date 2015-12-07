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

pub fn new_listener() -> mpsc::Sender<([u8; 4], SocketAddr)> {
    Command::new("/usr/local/bin/gpio").arg("-g").arg("mode").arg("24").arg("out").status().unwrap_or_else(|e| {
        panic!("failed to execute process: {}", e)
    });

    Command::new("/usr/local/bin/gpio").arg("-g").arg("write").arg("24").arg("1").status().unwrap_or_else(|e| {
        panic!("failed to execute process: {}", e)
    });

    let (tx, rx) = mpsc::channel();
    thread::Builder::new().name("plugin_gpio".to_string()).spawn(move || {
        for (data, _) in rx.iter() {
            let d: [u8; 4] = data;
            if d[0] == 0 && d[1] == 0 && d[2] == 0 {
                write(24, d[3]);
            }
        }
    }).unwrap();
    tx
}
