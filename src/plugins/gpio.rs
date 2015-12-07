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
    Command::new("/usr/local/bin/gpio -g mode 24 out && /usr/local/bin/gpio -g write 24 1").status().unwrap_or_else(|e| {
        panic!("failed to execute process: {}", e)
    });

    let (tx, rx) = mpsc::channel();
    thread::Builder::new().name("plugin_gpio".to_string()).spawn(move || {
        let gpio = GPIO::listen(rx);
        for data in gpio.rx.iter() {
            println!("GPIO RECEIVED SOME DATA ({})", data.0[0]);
            let id = data.0;
            if id[0] == 0 && id[1] == 0 && id[2] == 0 {
                println!("ID MATCHES! NOW DOING SMTH");
                Command::new(format!("/usr/local/bin/gpio -g mode 24 {}", !id[3])).status().unwrap_or_else(|e| {
                    panic!("failed to execute process: {}", e)
                });
            }
        }
    }).unwrap();
    tx
}
