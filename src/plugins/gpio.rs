use std::thread;
use std::sync::mpsc;
use std::net::SocketAddr;
use std::process::Command;

struct GPIO {
    rx: mpsc::Receiver<([u8; 4], SocketAddr)>,
}

impl GPIO {
    fn listen(rx: mpsc::Receiver<([u8; 4], SocketAddr)>) -> GPIO {
        let gpio = GPIO {
            rx: rx
        };
        gpio
    }
}

pub fn new_listener() -> mpsc::Sender<([u8; 4], SocketAddr)> {
    // Command::new("/usr/local/bin/gpio").arg("-g").arg("mode").arg("24").arg("out").status().unwrap_or_else(|e| {
    //     panic!("failed to execute process: {}", e)
    // });
    //
    // Command::new("/usr/local/bin/gpio").arg("-g").arg("write").arg("24").arg("1").status().unwrap_or_else(|e| {
    //     panic!("failed to execute process: {}", e)
    // });

    let (tx, rx) = mpsc::channel();
    thread::Builder::new().name("plugin_gpio".to_string()).spawn(move || {
        let gpio = GPIO::listen(rx);
        for data in gpio.rx.iter() {
            let id = data.0;
            if id[0] == 0 && id[1] == 0 && id[2] == 0 {
                let value = if id[3] == 0 { "1" } else { "0" };
                println!("{}", value);
                Command::new("/usr/local/bin/gpio").arg("-g").arg("write").arg("24").arg(value).status().unwrap_or_else(|e| {
                    panic!("failed to execute process: {}", e)
                });
            }
        }
    }).unwrap();
    tx
}
